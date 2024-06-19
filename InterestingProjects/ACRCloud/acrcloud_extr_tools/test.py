import os
import sys

from acrcloud.recognizer import ACRCloudRecognizer

if __name__ == "__main__":
    config = {
        # Replace "xxxxxxxx" below with your project's host, access_key and access_secret.
        "host": "identify-eu-west-1.acrcloud.com",
        "access_key": "f61631e400abaa655e5f193ae2b6ab56",
        "access_secret": "CnwrTyrAZStIyJZ1yiISbHmzmDJLjh22K5WdIvxW",
        "timeout": 10,  # seconds
    }

    """This module can recognize ACRCloud by most of audio/video file. 
        Audio: mp3, wav, m4a, flac, aac, amr, ape, ogg ...
        Video: mp4, mkv, wmv, flv, ts, avi ..."""
    re = ACRCloudRecognizer(config)

    # recognize by file path, and skip 0 seconds from from the beginning of sys.argv[1].
    print(re.recognize_by_file("./000/000002.mp3.mp3", 0))
