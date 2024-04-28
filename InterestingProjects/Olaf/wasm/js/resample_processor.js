let exports = []


class ResampleProcessor extends AudioWorkletProcessor {
    constructor(options){
        super(options);

        this.src = null;

        //We expect mono
        let nChannels = 1;
        let inputSampleRate = sampleRate;
        let outputSampleRate = 16000; //or another target sample rate

        exports.LibSampleRate.create(nChannels, inputSampleRate, outputSampleRate,{})
        .then((src) => {
            this.src = src;
            console.log(src);
        })
        .catch((err) => {
            // unable to find the WASM file. handle however you choose
            console.log(err);
        });
        
    }

    process(inputs,outputs,parameters){
        //mono first channel
        var audioInputBuffer = inputs[0][0];
        //http://www.mega-nerd.com/SRC/api_full.html
        if(this.src != null){
          let resampled = this.src.full(audioInputBuffer);
          //do something with the resampled audio, 
          let resampleRatio = resampled.length / audioInputBuffer.length;
          this.port.postMessage(resampleRatio)
          //this.port.postMessage(resampled)
        }

        return true;
    }
}

registerProcessor('resample-processor', ResampleProcessor);