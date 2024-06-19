"""
This is a demo program which implements ACRCloud Identify Protocol V1 with the third party library "requests".
We recomment you implement your own app with "requests" too.
You can install this python library by:
1) sudo easy_install requests 
2) sudo pip install requests
"""

import base64
import csv
import glob
import hashlib
import hmac
import json
import os
import shutil
import sys
import time

import requests
from pydub import AudioSegment
from tinytag import TinyTag

"""
Replace "###...###" below with your project's host, access_key and access_secret.
"""
access_key = "f61631e400abaa655e5f193ae2b6ab56"
access_secret = "CnwrTyrAZStIyJZ1yiISbHmzmDJLjh22K5WdIvxW"
requrl = "https://identify-eu-west-1.acrcloud.com/v1/identify"

http_method = "POST"
http_uri = "/v1/identify"
# default is "fingerprint", it's for recognizing fingerprint,
# if you want to identify audio, please change data_type="audio"
data_type = "audio"
signature_version = "1"
timestamp = time.time()

string_to_sign = (
    http_method
    + "\n"
    + http_uri
    + "\n"
    + access_key
    + "\n"
    + data_type
    + "\n"
    + signature_version
    + "\n"
    + str(timestamp)
)

sign = base64.b64encode(
    hmac.new(
        access_secret.encode("ascii"),
        string_to_sign.encode("ascii"),
        digestmod=hashlib.sha1,
    ).digest()
).decode("ascii")

with open("output_cloudT.csv", "a", newline="") as csvfile:
    fieldnames = ["file", "type", "length", "modification", "result", "match", ""]
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()

times = [15, 10, 5, 2]
mp3_files = sorted(glob.glob("000/*.mp3", recursive=False))
# suported file formats: mp3,wav,wma,amr,ogg, ape,acc,spx,m4a,mp4,FLAC, etc
# File size: < 1M , You'de better cut large file to small file, within 15 seconds data size is better
for t in times:
    for mp3 in mp3_files[:611]:
        print(mp3)

        name = mp3.replace(".mp3", "")
        name = name.replace("000/", "")
        song = AudioSegment.from_file(mp3, format="mp3")
        seconds = t * 1000
        clip = song[:seconds]

        if not os.path.exists("temp_files/"):
            os.makedirs("temp_files/")
        new_name = "temp_files/" + name + "_" + ".mp3"
        clip.export(new_name, format="mp3")
        # os.system('./acrcloud_extr_linux -cli -l {t} -i {mp3} -o "temp_files/"')

        # f = open(mp3, "rb")
        sample_bytes = os.path.getsize(new_name)

        files = [("sample", (new_name, open(new_name, "rb"), "audio/mpeg"))]

        data = {
            "access_key": access_key,
            "sample_bytes": sample_bytes,
            "timestamp": str(timestamp),
            "signature": sign,
            "data_type": data_type,
            "signature_version": signature_version,
        }

        r = requests.post(requrl, files=files, data=data)
        r.encoding = "utf-8"
        d = r.json()

        tag = TinyTag.get(mp3)

        if d["status"]["msg"] == "Success":
            with open("output_cloudT.csv", "a", newline="") as csvfile:
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                writer.writerow(
                    {
                        "file": name,
                        "type": tag.genre,
                        "length": str(t),
                        "modification": "original",
                        "result": d["status"]["msg"],
                        "match": d["metadata"]["custom_files"][0]["title"],
                    }
                )
        else:
            with open("output_cloudT.csv", "a", newline="") as csvfile:
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                writer.writerow(
                    {
                        "file": name,
                        "type": tag.genre,
                        "length": str(t),
                        "modification": "original",
                        "result": d["status"]["msg"],
                        "match": "None",
                    }
                )
    shutil.rmtree("./temp_files")
