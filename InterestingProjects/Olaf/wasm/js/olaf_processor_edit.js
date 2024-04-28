
// Resample the Secret Rabit Code: https://github.com/aolsenjazz/libsamplerate-js

// To post messages in the callback
let processor = null;

//The sample rate expected by Olaf
const targetSampleRate = 16000;

//Callback called when Olaf finds a match
function olaf_fp_matcher_callback(matchCount, queryStart, queryStop, path, matchIdentifier, referenceStart, referenceStop){
	//console.log("js",matchCount, queryStart, queryStop, path, matchIdentifier, referenceStart, referenceStop);
	//convert string
	processor.port.postMessage({
		match_count: matchCount,
		query_time_start: queryStart,
		query_time_stop: queryStop,
		match_name: path,
		match_id: matchIdentifier,
		reference_time_start: referenceStart,
		reference_time_stop: referenceStop});
}

//The audio worklet
class OlafProcessor extends AudioWorkletProcessor {

	constructor(options){
		super(options);

		let nChannels = 1;
		let inputSampleRate = sampleRate;
		let outputSampleRate = targetSampleRate;

		//create the secret rabit code resampler
		exports.LibSampleRate.create(nChannels, inputSampleRate, outputSampleRate,{})
		.then((src) => {
			this.src = src;
			console.log(src);
		})
		.catch((err) => {
			// unable to find the WASM file. handle however you choose
			console.log(err);
		});
		
		//Create the olaf module
		Module().then(olaf => {
			this.olaf = olaf;
			this.js_wrapped_olaf_fingerprint_match = this.olaf.cwrap("olaf_fingerprint_match", "number" , ["number","number","number","number"]);
			this.inputAudioBytes = 1024 * 4;
			this.inputAudioPtr = this.olaf._malloc(this.inputAudioBytes);

			//256 fingrprints, 8 bytes per long, 5 longs per print
			this.fingerprintsBytes = 256 * 8 * 5;
			this.fingerprintPtr = this.olaf._malloc(this.fingerprintsBytes);
		});

		processor = this;	
	}

	//Processes mono audio
	process(inputs,outputs,parameters){

		//wait for the modules to initialize, ignore when not ready
		if(this.olaf == null || this.src == null){
			return true;
		}//else

		//mono first channel
		var audioInputBuffer = inputs[0][0];

		let resampledInputBuffer = audioInputBuffer

		//only resample when needed, the audio context is not already 
		//at the target sample rate
		if(targetSampleRate != sampleRate){
			resampledInputBuffer = this.src.full(audioInputBuffer);
		}

		var dataHeap = new Uint8Array(this.olaf.HEAPU8.buffer, this.inputAudioPtr, this.inputAudioBytes );
		dataHeap.set(new Uint8Array(resampledInputBuffer.buffer));

		var fingerprintHeap = new Uint8Array(this.olaf.HEAPU8.buffer, this.fingerprintPtr, this.fingerprintsBytes );
		var fingerprintBuffer = new Uint8Array(this.fingerprintsBytes);

		fingerprintHeap.set(new Uint8Array(this.fingerprintBuffer));

		var audioBlockIndex = this.js_wrapped_olaf_fingerprint_match(dataHeap.byteOffset,resampledInputBuffer.length,fingerprintHeap.byteOffset,128);

		return true;
	}
}

registerProcessor('olaf-processor', OlafProcessor);