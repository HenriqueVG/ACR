# -*- coding: utf-8 -*-
"""onset_detection.ipynb

Automatically generated by Colab.

Original file is located at
    https://colab.research.google.com/github/iranroman/musicinformationretrieval.com/blob/gh-pages/onset_detection.ipynb
"""

# Commented out IPython magic to ensure Python compatibility.
# %matplotlib inline
import numpy, scipy, matplotlib.pyplot as plt, IPython.display as ipd
import librosa, librosa.display
import stanford_mir; stanford_mir.init()

plt.style.use('seaborn-muted')
plt.rcParams['figure.figsize'] = (14, 5)
plt.rcParams['axes.grid'] = True
plt.rcParams['axes.spines.left'] = False
plt.rcParams['axes.spines.right'] = False
plt.rcParams['axes.spines.bottom'] = False
plt.rcParams['axes.spines.top'] = False
plt.rcParams['axes.xmargin'] = 0
plt.rcParams['axes.ymargin'] = 0
plt.rcParams['image.cmap'] = 'gray'
plt.rcParams['image.interpolation'] = None

"""[&larr; Back to Index](index.html)

# Onset Detection

Automatic detection of musical events in an audio signal is one of the most fundamental tasks in music information retrieval. Here, we will show how to detect an **onset**, the very instant that marks the beginning of the transient part of a sound, or the earliest moment at which a transient can be reliably detected.

For more reading:
- [Tutorial on Onset Detection by Juan Bello](https://files.nyu.edu/jb2843/public/Publications_files/2005_BelloEtAl_IEEE_TSALP.pdf)
- [Boeck, Sebastian, Florian Krebs, and Markus Schedl. “Evaluating the Online Capabilities of Onset Detection Methods.” ISMIR 2012](http://www.cp.jku.at/research/papers/Boeck_etal_ISMIR_2012.pdf)

Load an audio file into the NumPy array `x` and sampling rate `sr`.
"""

x, sr = librosa.load('audio/classic_rock_beat.wav')

ipd.Audio(x, rate=sr)

plt.figure(figsize=(14, 5))
librosa.display.waveplot(x, sr)

"""## `librosa.onset.onset_detect`

[`librosa.onset.onset_detect`](http://librosa.github.io/librosa/generated/librosa.onset.onset_detect.html#librosa.onset.onset_detect) works in the following way:

1.  Compute a spectral novelty function.
2.  Find peaks in the spectral novelty function.
3.  [optional] Backtrack from each peak to a preceding local minimum. Backtracking can be useful for finding segmentation points such that the onset occurs shortly after the beginning of the segment.

Compute the frame indices for estimated onsets in a signal:
"""

onset_frames = librosa.onset.onset_detect(x, sr=sr, wait=1, pre_avg=1, post_avg=1, pre_max=1, post_max=1)
print(onset_frames) # frame numbers of estimated onsets

"""Convert onsets to units of seconds:"""

onset_times = librosa.frames_to_time(onset_frames)
print(onset_times)

"""Plot the onsets on top of a spectrogram of the audio:"""

S = librosa.stft(x)
logS = librosa.amplitude_to_db(abs(S))

plt.figure(figsize=(14, 5))
librosa.display.specshow(logS, sr=sr, x_axis='time', y_axis='log', cmap='Reds')
plt.vlines(onset_times, 0, 10000, color='#3333FF')

"""Let's also plot the onsets with the time-domain waveform."""

plt.figure(figsize=(14, 5))
librosa.display.waveplot(x, sr=sr)
plt.vlines(onset_times, -0.8, 0.79, color='r', alpha=0.8)

"""## librosa.clicks

We can add a click at the location of each detected onset.
"""

clicks = librosa.clicks(frames=onset_frames, sr=sr, length=len(x))

"""Listen to the original audio plus the detected onsets. One way is to add the signals together, sample-wise:"""

ipd.Audio(x + clicks, rate=sr)

"""Another method is to play the original track in one stereo channel and the click track in the other stereo channel:"""

ipd.Audio(numpy.vstack([x, clicks]), rate=sr)

"""You can also change the click to a custom audio file instead:"""

cowbell, _ = librosa.load('audio/cowbell.wav')

ipd.Audio(cowbell, rate=sr)

"""More cowbell?"""

clicks = librosa.clicks(frames=onset_frames, sr=sr, length=len(x), click=cowbell)

ipd.Audio(x + clicks, rate=sr)

ipd.Image(url='http://www.reactiongifs.com/r/2013/11/More-cowbell.gif')

"""## Questions

In `librosa.onset.onset_detect`, use the `backtrack=True` parameter. What does that do, and how does it affect the detected onsets? (See [`librosa.onset.onset_backtrack`](http://librosa.github.io/librosa/generated/librosa.onset.onset_backtrack.html#).)

In `librosa.onset.onset_detect`, you can use the keyword parameters found in [`librosa.util.peak_pick`](http://librosa.github.io/librosa/generated/librosa.util.peak_pick.html#librosa.util.peak_pick), e.g. `pre_max`, `post_max`, `pre_avg`, `post_avg`, `delta`, and `wait`, to control the peak picking algorithm. Adjust these parameters. How does it affect the detected onsets?

[&larr; Back to Index](index.html)
"""