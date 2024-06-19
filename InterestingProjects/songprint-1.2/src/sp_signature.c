/*  sp_signature.c - Calculates signature based on waveform
 *  Copyright (C) 2000  eTantrum, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#ifdef HAVE_STRING_H
#include <string.h>                /* for g_memmove */
#endif

#include <glib.h>
#include <math.h>
#include "sig_tables.h"
#include "sp_types.h"
#include "sp_signature.h"

/* How many seconds our sig operates over */
static const guint NUMBER_OF_SECONDS = 30;

/**
 *  _sig_new:
 *  @song_audio_format  Audio encoding details 
 *
 *  Allocates and initializes a state structure for holding all 
 *  the data we need to maintain, and precalculates some values 
 *  we need later.
 *
 *  Returns a newly allocated _sig_state_t.
 */
static _sig_state_t *
_sig_new (sp_audio_info_t * song_audio_format)
{
  const guint START_OFFSET = 0;
  gint i;

  _sig_state_t *state = g_malloc0 (sizeof (_sig_state_t));

  /* create fftw plan */
  state->plan = rfftw_create_plan (NUM_FFT_SAMPLES, FFTW_FORWARD,
                                   FFTW_ESTIMATE);

  /* ugly calculations needed to decode */
  state->decode_data.sample_size =
    (song_audio_format->bits_per_sample +
     7) / 8 * song_audio_format->num_channels;

  state->decode_data.buffer_size = NUM_FFT_SAMPLES
    * state->decode_data.sample_size;

  state->decode_data.sample_offset = START_OFFSET;
  state->decode_data.total_bytes_decoded = 0;
  state->decode_data.found_content = FALSE;

  state->decode_data.raw_pcm_data =
    (guchar *) g_malloc (state->decode_data.buffer_size);

  /* ugly initialization for sig generation code */
  state->format = song_audio_format;
  state->num_blocks = 0;
  state->blocks_allocated = NUMBER_OF_SECONDS *
    song_audio_format->sample_rate / NUM_FFT_SAMPLES;

  for (i = 0; i < NUM_FFT_FINAL_BANDS; i++)
    {
      state->freq_blocks[i] = g_malloc (state->blocks_allocated *
                                        sizeof (gulong));
    }

  /* ugly return */
  return state;
}

/**
 *  _sig_destroy:
 *  @state    A previously allocated state structure
 *
 *  Frees all memory associated with the state variable.
 */
static void
_sig_destroy (_sig_state_t * state)
{
  gint i;

  rfftw_destroy_plan (state->plan);
  g_free (state->decode_data.raw_pcm_data);

  for (i = 0; i < NUM_FFT_FINAL_BANDS; i++)
    {
      g_free (state->freq_blocks[i]);
    }
  g_free (state);
}

/**
 *  _mean:
 *  @n       Number of elements to average
 *  @values  Values to average
 *
 *  Computes arithmetic mean.
 *
 *  Returns the mean of @values. 
 */
static gdouble
_mean (gint n, gulong *values)
{
  gint i;
  gdouble sum = 0;

  for (i = 0; i < n; i++)
    sum += values[i];

  return (sum / (double) n);
}

/**
 *  _std_deviation:
 *  @n       Number of elements 
 *  @values  Values from which std deviation is taken
 *  @mean    Mean of @values
 *
 *  Computes standard deviation of @values.
 *
 *  Returns std deviation.
 */
static gdouble
_std_deviation (gint n, gulong *values, gdouble mean)
{
  gint i;
  gdouble sum = 0;

  for (i = 0; i < n; i++)
    sum += (values[i] - mean) * (values[i] - mean);

  sum /= (double) n;

  return (sqrt (sum));
}


/**
 *  _construct_sample:
 *  @sample_buf     Pointer to the sample  
 *  @sample_size    Size of sample in bits  
 *
 *  Creates a new sample in host byte order from the 
 *  little-endian sample pointed to by @sample_buf. 
 *  
 *  Returns the host byte order sample. 
 */
static gint
_construct_sample (guchar * sample_buf, guint sample_size)
{
  gint i;
  gint sample = 0;
  gint num_bytes = (sample_size + 7) / 8;
  gint bit_count = 0;

  g_assert(num_bytes < sizeof(gint));

  for (i = 0; i < num_bytes; i++)
    {
      sample |= (*sample_buf) << bit_count;
      bit_count += 8;
      sample_buf++;
    }

  /* do sign extension */
  if (sample & (1 << (sample_size - 1)))
    {
      sample |= (-1 << (sample_size - 1));
    }
  return sample;
}

/**
 *  _split_into_bands:
 *  @state         State variable for audio encoding, etc
 *  @samples       Input waveform
 *  @output_bands  A previously allocated output buffer 
 *
 *  This function does an FFT and some averaging on the waveform
 *  in @samples to split the audio into %NUM_FFT_FINAL_BANDS 
 *  frequency bands.  Resulting bands are stored in @output_bands.
 */
static void
_split_into_bands (_sig_state_t * state, gdouble *samples,
                   gulong * output_bands)
{
  gint i, j;
  gint num_bands;
  const gint band_shift = 8.0;
  gdouble bands[NUM_FFT_SAMPLES];
  gint freq_index;
  gdouble freq_samples[NUM_FFT_SAMPLES];
  gdouble log_scale[] = { 1, 1, 2, 3, 4, 6, 9, 14, 20, 30,
                          44, 64, 94, 137, 201, 294 };

  glong band_avg;

  num_bands = NUM_FFT_SAMPLES / 2;

  /* raw fft */
  rfftw_one (state->plan, samples, bands);

  /* collate the components of the freq data */
  for (i = 0; i < num_bands; i++)
    {
      gdouble freq_mag = bands[i] * bands[i] +
        bands[2 * num_bands - i - 1] * bands[2 * num_bands - i - 1];

      if (freq_mag <= 0)
        {
          freq_samples[i] = 0;
        }
      else
        {
          freq_samples[i] = log10 (freq_mag / NUM_FFT_SAMPLES_SQ) + band_shift;
        }
    }

  /* discard lowest band */
  freq_index = 1;

  /* reduce to NUM_FFT_FINAL_BANDS bands */
  for (i = 0; i < NUM_FFT_FINAL_BANDS; i++)
    {
      band_avg = 0;
      for (j = 0; j < log_scale[i]; j++)
        {
          band_avg += (glong) (freq_samples[freq_index++] * 12.0);
        }

      band_avg /= log_scale[i];

      band_avg /= 2;
      if (band_avg < 0)
        band_avg = 0;
      if (band_avg > 63)
        band_avg = 63;

      output_bands[i] = band_avg;
    }
}

/**
 *  _count_waveform:
 *  @state    State variable
 *  @samples  Array of pcm data     
 *
 *  Generates %NUM_FFT_FINAL_BANDS frequency bands for this chunk.
 */
static void
_count_waveform (_sig_state_t * state, guchar * samples)
{
  gdouble dsamp[NUM_FFT_SAMPLES];
  gulong bands[NUM_FFT_FINAL_BANDS];
  gint i, j;
  sp_audio_info_t *format = state->format;
  gint bytes_per_sample;

  bytes_per_sample = (format->bits_per_sample + 7) / 8;

  /* copy samples ginto an array of doubles for fft */
  for (i = 0; i < NUM_FFT_SAMPLES; i++)
    {
      dsamp[i] = 0;

      /* average channels and normalize the sample */
      for (j = 0; j < format->num_channels; j++)
        {
          dsamp[i] += (double) _construct_sample (samples,
                                                  format->bits_per_sample);
          samples += bytes_per_sample;
        }
      dsamp[i] /= (double) format->num_channels
        * (double) (1 << (format->bits_per_sample - 1));

      /* scale by gaussian */
      dsamp[i] *= _sp_sig_pcm_envelope[i];
    }

  state->num_blocks++;

  if (state->num_blocks > state->blocks_allocated)
    {
      state->blocks_allocated += 10;
      for (i = 0; i < NUM_FFT_FINAL_BANDS; i++)
        {
          state->freq_blocks[i] = g_realloc (state->freq_blocks[i],
                                             state->blocks_allocated *
                                             sizeof (gulong));
        }
    }

  _split_into_bands (state, dsamp, bands);

  /* bands are column-major in final array */
  for (i = 0; i < NUM_FFT_FINAL_BANDS; i++)
    {
      state->freq_blocks[i][state->num_blocks - 1] = bands[i];
    }
}

/**
 *  _find_useful_pcm_content:
 *  @found_content  Boolean updated when data found
 *  @pcmdata        PCM buffer
 *  @buffer_size    Size of @pcmdata
 *  @sample_size    Size of a multichannel sample
 *
 *  Scans for the first sample with non-zero data and
 *  sets *@found_content to true when it is done.
 *
 *  Returns the index of the sample.
 */
static guint
_find_useful_pcm_content (gboolean * found_content, guchar * pcmdata,
                          guint buffer_size, guint sample_size)
{
  guint current_skip_amount = 0;
  while (current_skip_amount < buffer_size
         && pcmdata[current_skip_amount] == 0)
    {
      current_skip_amount++;
    }

  if (current_skip_amount < buffer_size)
    *found_content = TRUE;

  /* valid data starts somewhere past the first sample? */
  current_skip_amount -= (current_skip_amount % (sample_size));

  return current_skip_amount;
}

/**
 *  _get_pcm_data_block:
 *  @state          State variable
 *  @get_pcm_data   A callback to procure more data
 *  @user_data      Optional parameter for get_pcm_data          
 *
 *  Gets a block of pcm data using @get_pcm_data.  Any 
 *  leading zeroed samples are skipped.
 *
 *  Returns the actual number of bytes read.
 */
static guint
_get_pcm_data_block (_sig_state_t * state,
                     guint (*get_pcm_data) (guint sample_offset,
                                            guint num_samples,
                                            guchar * output_buf,
                                            void *user_data), 
                     void *user_data)
{
  _sig_state_decode_data_t *decode_data = &state->decode_data;

  guint num_bytes_to_read = state->decode_data.buffer_size;
  guint current_skip_amount;
  guint bytes_decoded = 0;
  guint valid_bytes_decoded = 0;
  gboolean done = FALSE;
  guint num_samples_to_read;
  gint buf_offset;

  while (!done && (num_bytes_to_read > 0))
    {
      num_samples_to_read = num_bytes_to_read / decode_data->sample_size;
      buf_offset = decode_data->buffer_size - num_bytes_to_read;

      bytes_decoded = get_pcm_data (decode_data->sample_offset,
                                    num_samples_to_read,
                                    &decode_data->raw_pcm_data[buf_offset],
                                    user_data);

      /* did it return less bytes than we requested? we must be at EOF */
      if (bytes_decoded < num_bytes_to_read)
        done = TRUE;

      num_bytes_to_read -= bytes_decoded;
      decode_data->sample_offset += num_samples_to_read;

      /* have we found actual audio data yet? */
      if (!decode_data->found_content)
        {

          current_skip_amount =
            _find_useful_pcm_content (&decode_data->found_content,
                                      decode_data->raw_pcm_data,
                                      decode_data->buffer_size,
                                      decode_data->sample_size);

          if (current_skip_amount < decode_data->buffer_size)
            {
              g_memmove (decode_data->raw_pcm_data,
                         decode_data->raw_pcm_data + current_skip_amount,
                         decode_data->buffer_size - current_skip_amount);
            }
          num_bytes_to_read = current_skip_amount;

          valid_bytes_decoded +=
            decode_data->buffer_size - current_skip_amount;
        }
      else
        {
          valid_bytes_decoded += bytes_decoded;
        }
    }

  decode_data->total_bytes_decoded += valid_bytes_decoded;

  /* return the number of bytes for samples decoded & _kept_ */
  return valid_bytes_decoded;
}

/**
 *  _create_sig:
 *  @state    State variable
 *  @sig      Signature structure to fill in
 *
 *  Averages all frequency bands and does standard 
 *  deviations on the bands in state->freq_blocks.  
 *  The results are normalized and stored in @sig.
 */
static void
_create_sig (_sig_state_t * state, sp_signature_t * sig)
{
  gint i;
  gdouble means[NUM_FFT_FINAL_BANDS];
  gdouble devs[NUM_FFT_FINAL_BANDS];
  gdouble mean_mean = 0;

  /* find means and normalizer */
  for (i = 0; i < NUM_FFT_FINAL_BANDS; i++)
    {
      means[i] = _mean (state->num_blocks, state->freq_blocks[i]);
      devs[i] = _std_deviation (state->num_blocks, state->freq_blocks[i],
                                means[i]);

      mean_mean += means[i];
    }
  mean_mean /= NUM_FFT_FINAL_BANDS;

  /* normalize means and std devs */
  for (i = 0; i < NUM_FFT_FINAL_BANDS; i++)
    {
      /* yes, we mean 65534 (for partitioning scheme on server) */
      means[i] = MIN ((gint) ((means[i] / mean_mean) * 32768), 65534);
      devs[i] = MIN ((gint) ((devs[i] / mean_mean) * 32768), 65534);
    }

  for (i = 0; i < NUM_FFT_FINAL_BANDS; i++)
    {
      sig->data[i] = (guint16) means[i];
      sig->data[i + NUM_FFT_FINAL_BANDS] = (guint16) devs[i];
    }
}

/**
 *  sp_generate_signature:
 *  @song_audio_format   Audio format of pcm data stream  
 *  @signature           Structure to which signature is written
 *  @get_pcm_data        Callback to get samples from pcm data stream
 *  @user_data           Caller-defined parameter to pcm callback
 *
 *  Generates a signature for a waveform.  All pcm data is procured 
 *  by calling @get_pcm_data, and the resulting signature is stored
 *  in @signature.  
 *
 *  Returns SP_SUCCESS if a signature was successfully generated.
 */
sp_rv
sp_generate_signature (sp_audio_info_t * song_audio_format,
                       sp_signature_t * signature,
                       guint (*get_pcm_data) (guint sample_offset,
                                              guint num_samples,
                                              guchar * output_buf,
                                              void *user_data),
                       void *user_data)
{
  _sig_state_t *state;
  guint ideal_total_bytes;
  guint total_bytes = 0;
  gboolean done = FALSE;
  guint result;

  g_assert (song_audio_format != NULL);
  g_assert (signature != NULL);
  g_return_val_if_fail(song_audio_format->bits_per_sample > 0, SP_FAILURE);

  state = _sig_new (song_audio_format);

  ideal_total_bytes = NUMBER_OF_SECONDS * song_audio_format->sample_rate
    * state->decode_data.sample_size;

  do
    {
      result = _get_pcm_data_block (state, get_pcm_data, user_data);
      if (result < state->decode_data.buffer_size)
        done = TRUE;

      total_bytes += result;

      /* Last chunk is discarded */
      if (!done && total_bytes < ideal_total_bytes)
        {
          _count_waveform (state, state->decode_data.raw_pcm_data);
        }

    }
  while (!done && (total_bytes < ideal_total_bytes));

  _create_sig (state, signature);

  _sig_destroy (state);

  return SP_SUCCESS;
}
