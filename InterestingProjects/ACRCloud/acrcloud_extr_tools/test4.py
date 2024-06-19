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


# change the pitch of a file without affecting tempo
def create_pitch_shifted_file(input_file, target_file, cents):
    command = 'sox "%s" "%s" pitch %s' % (input_file, target_file, cents)
    os.system(command)


# factor gives the ratio of new tempo to the old tempo, so e.g. 1.1 speeds up the tempo by 10%, and 0.9 slows it down by 10%.
def change_tempo(input_file, target_file, factor):
    command = 'sox "%s" "%s" tempo %s' % (input_file, target_file, factor)
    os.system(command)


# Apply a flanging effect to the audio
def create_flanger_file(input_file, target_file):
    command = 'sox "%s" "%s" flanger' % (input_file, target_file)
    os.system(command)


def create_fm_compressed_file(input_file, target_file):
    command = (
        'sox "%s" "%s" gain -15 sinc  -b 29  -n 100 -8000 mcompand \
                   "0.005,0.1 -47,-40,-34,-34,-17,-33" 100 \
                   "0.003,0.05 -47,-40,-34,-34,-17,-33" 400 \
                   "0.000625,0.0125 -47,-40,-34,-34,-15,-33" 1600 \
                   "0.0001,0.025 -47,-40,-34,-34,-31,-31,-0,-30" 6400 \
                   "0,0.025 -38,-31,-28,-28,-0,-25" \
                   gain 15 highpass 22 highpass 22 sinc -n 255 -b 16 -17500 \
                   gain 9 lowpass -1 17801'
        % (input_file, target_file)
    )
    os.system(command)


# Adjust the audio speed (pitch and tempo together). factor is either the ratio of
# the new speed to the old speed: greater than 1 speeds up, less than 1 slows down
def create_sped_up_file(input_file, target_file, factor):
    command = 'sox "%s" "%s" speed %s' % (input_file, target_file, factor)
    os.system(command)


# Apply a band pass filter
def band_pass_filter(input_file, target_file, center):
    command = 'sox "%s" "%s" band %s' % (input_file, target_file, center)
    os.system(command)


# Apply a corus effect filter
def chorus(input_file, target_file):
    command = 'sox "%s" "%s" chorus 0.7 0.9 55 0.4 0.25 2 -t' % (
        input_file,
        target_file,
    )
    os.system(command)


# Apply an echo effect filter
def echo(input_file, target_file):
    command = 'sox "%s" "%s" echo 0.8 0.9 500 0.3' % (input_file, target_file)
    os.system(command)


# Apply a tremolo effect filter
def tremolo(input_file, target_file):
    command = 'sox "%s" "%s" tremolo 8' % (input_file, target_file)
    os.system(command)


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

with open("output_cloudMP.csv", "a", newline="") as csvfile:
    fieldnames = ["file", "type", "length", "modification", "result", "match"]
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()

times = [10]
mp3_files = sorted(glob.glob("000/*.mp3", recursive=False))
# suported file formats: mp3,wav,wma,amr,ogg, ape,acc,spx,m4a,mp4,FLAC, etc
# File size: < 1M , You'de better cut large file to small file, within 15 seconds data size is better
for t in times:
    for mp3 in mp3_files[:611]:
        print(mp3)
        tag = TinyTag.get(mp3)

        name = mp3.replace(".mp3", "")
        name = name.replace("000/", "")
        song = AudioSegment.from_file(mp3, format="mp3")
        seconds = t * 1000
        clip = song[:seconds]

        if not os.path.exists("temp_files/"):
            os.makedirs("temp_files/")
        new_name = "temp_files/" + name + "_" + ".mp3"
        flanger_f = "temp_files/" + name + "_" + "flanger.mp3"
        chorus_f = "temp_files/" + name + "_" + "chorus.mp3"
        echo_f = "temp_files/" + name + "_" + "echo.mp3"
        band_f = "temp_files/" + name + "_" + "band.mp3"

        clip.export(new_name, format="mp3")

        create_flanger_file(new_name, flanger_f)
        band_pass_filter(new_name, band_f, 2000)
        chorus(new_name, chorus_f)
        echo(new_name, echo_f)

        # f = open(mp3, "rb")
        sample_bytes = os.path.getsize(flanger_f)
        files = [("sample", (flanger_f, open(flanger_f, "rb"), "audio/mpeg"))]
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
        print(flanger_f)
        d = r.json()

        if d["status"]["msg"] == "Success":
            with open("output_cloudMP.csv", "a", newline="") as csvfile:
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
            with open("output_cloudMP.csv", "a", newline="") as csvfile:
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

        sample_bytes = os.path.getsize(echo_f)
        files = [("sample", (echo_f, open(echo_f, "rb"), "audio/mpeg"))]
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
        print(echo_f)
        print(r.text)
        d = r.json()

        if d["status"]["msg"] == "Success":
            with open("output_cloudMP.csv", "a", newline="") as csvfile:
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
            with open("output_cloudMP.csv", "a", newline="") as csvfile:
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

        sample_bytes = os.path.getsize(band_f)
        files = [("sample", (band_f, open(band_f, "rb"), "audio/mpeg"))]
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
        print(band_f)
        print(r.text)

        if d["status"]["msg"] == "Success":
            with open("output_cloudMP.csv", "a", newline="") as csvfile:
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
            with open("output_cloudMP.csv", "a", newline="") as csvfile:
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

        sample_bytes = os.path.getsize(chorus_f)
        files = [("sample", (chorus_f, open(chorus_f, "rb"), "audio/mpeg"))]
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
        print(chorus_f)
        print(r.text)

        if d["status"]["msg"] == "Success":
            with open("output_cloudMP.csv", "a", newline="") as csvfile:
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
            with open("output_cloudMP.csv", "a", newline="") as csvfile:
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
