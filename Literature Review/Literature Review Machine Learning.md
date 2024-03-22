#machineLearning #Notes 

# Fingerprinting

## Contrastive Unsupervised Learning for Audio Fingerprinting

Authors: Zhesong Yu, Xingjian Du, Bilei Zhu, Zejun Ma
[Semantic Scholar](https://www.semanticscholar.org/paper/Contrastive-Unsupervised-Learning-for-Audio-Yu-Du/00e08bc8f221cec65c4b7e315e633801c258a0f0) 2020

### Abstract 
The paper discusses applying contrastive learning to audio fingerprinting, treating distorted versions of the same audio track as similar and different tracks as dissimilar, proposing a method based on the MoCo framework that shows effectiveness in audio identification, even with challenging distortions.
### Methodology
The methodology involved introducing contrastive learning to audio fingerprinting, utilizing the momentum contrast (MoCo) framework, and evaluating the method's effectiveness and robustness through experiments.
### Outcome measured
The main outcome measured in the study is the effectiveness of the contrastive learning method for audio fingerprinting (AFP) in audio identification, with robustness to serious audio distortions such as speed change and pitch shifting.

##  Neural Audio Fingerprint for High-Specific Audio Retrieval Based on Contrastive Learning

Authors: Sungkyun Chang, Donmoon Lee, Jeongsoon Park, Hyungui Lim, Kyogu Lee, Karam Ko, Yoonchang Han
[IEEExplore](https://ieeexplore.ieee.org/document/9414337) 2021

### Abstract 
A system using 10x smaller storage has shown promising results in the segment-level search task.
Code and dataset are available at https://mimbres.github.io/neural-audio-fp/.
### Methodology
Involves using a low-dimensional representation from short audio segments, employing a contrastive learning framework, utilizing data augmentation methods, transforming acoustic features into L2-normalized space, and testing the generality of learned embeddings through a downstream task.
### Outcome Measured
Performance in segment/song-level search and optimization for segment-level search

## Audio-Fingerprinting via Dictionary Learning

Authors: Christina Saravanos, D. Ampeliotis, K. Berberidis
[Semantic Scholar](https://www.semanticscholar.org/paper/Audio-Fingerprinting-via-Dictionary-Learning-Saravanos-Ampeliotis/ddcc5385d92ee2aaa4aa4e77fa632a0a61b5b022) 2020
### Abstract Summary
A proposed audio-fingerprinting scheme is able to construct a unique and concise representation of an audio signal by applying a dictionary.
### Methodology
The methodology involves using the K-SVD algorithm on a song database to create a unique audio fingerprinting scheme that can identify audio signal content, even in noisy conditions, outperforming a Shazam-based paradigm.
### Outcome Measured
The promising results which emerged while conducting the experiments suggested that, not only the proposed approach preformed rather well in its attempt to identify the signal content of several audio clips –even in cases this content had been distorted by noise - but also surpassed the recognition rate of a Shazam-based paradigm.
### Limitation
The proposed scheme is not yet able to handle audio signals that have been subjected to time-scale modifications.


## Recurrent neural network based Music Recognition using Audio Fingerprinting

Authors: G. Deepsheka, R. Kheerthana, M. Mourina, B. Bharathi 
[IEEExplore](https://ieeexplore.ieee.org/document/9214302) 2020

### Abstract Summary
The aim of this research is to obtain an efficient aid for the people to discover the song and get acquainted with it and to provide the original song as the output based on a score which is the number of matching features from both the originalsong and its cover version.

### Methodology
Audio fingerprinting and matching module with linear search against database. 
Feature extraction like chroma, spectral centroid, MFCC, and tempo. 
Long Short Term Memory (LSTM) implementation for song output scoring. 
Lyrical matching for singing detection and translation to text.

### Outcome Measured
The representation is robust to timbral changes, key changes and small local tempo deviations. The similarity in time-series representations is being looked and extracted a distance measure factor which is invariant to the structural changes. It is noted that adaptive thresholding is an important step in the pre-processing step and the method for _Singing Detection_ is based on mere concept of voice to text translation and the subsequent string matching called lyrical matching.

# Denoising

## Audio Denoising for Robust Audio Fingerprinting

Authors: Kamil Akesbi
[arXiv.org](https://arxiv.org/abs/2212.11277) 2022

### Abstract Summary
In this paper , a denoising model was proposed to limit the impact of background noise on the extracted spectral peaks, improving the robustness of audio fingerprinting to background noise in real environments.

### Methodology
The researchers propose a hybrid strategy that combines a denoising deep learning (DL) model with a peak-based audio fingerprinting (AFP) algorithm
They simulate noisy music recordings using a realistic data augmentation pipeline.
A DL model is trained to denoise the noisy recordings, which helps limit the impact of background noise on the AFP system's extracted peaks.
The researchers also propose a novel loss function to adapt the DL model to the considered AFP system, increasing its precision in terms of retrieved spectral peaks.

### Results
Altough our deep learning models show encouraging results in terms of capacity to denoise CQTs and ability to preserve spectral peaks, we still haven’t study the impact of their integration in the AFP system’s identification rate. By increasing the percentage of retrieved spectral peaks belonging to clean music references, we should logically increase the system’s identification rate. However, establishing the exact relation between these two metrics would require more investigations.

## Potential Barriers to Music Fingerprinting Algorithms in the Presence of Background Noise

Authors: Zahid Mehmood; Khurram Ashfaq Qazi; Muhammad Tahir; Rehan Muhammad Yousaf; Muhammad Sardaraz
[IEEExplore](https://ieeexplore.ieee.org/document/9044274) 2020

### Abstract Summary
A comprehensive and powerful survey of already developed algorithms for music fingerprinting algorithms is conducted and shows that the results of music fingerprint classification are more successful when deep learning techniques for classification are used.

### Methodology

Conducted a comprehensive survey of already developed algorithms for generating acoustic fingerprints. 
Evaluated four major music fingerprinting algorithms to identify and analyze potential hurdles that can affect their results. 
Performed behavioral analysis of fingerprinting algorithms using audio samples of different languages and under different environmental conditions to assess the impact of background and environmental noise on the efficiency of music fingerprinting algorithms. **
Tested the acoustic feature modeling and music fingerprinting algorithms using the standard datasets of iKala, MusicBrainz, and MIR-1K.

### Results
Music fingerprinting algorithms could be improved using a layered approach, while acoustic feature classification could be improved using deep neural network algorithms.

# Audio Classification

## Deep neural net based filter prediction for audio event classification and extraction
Authors: Erik Visser, Yinyi Guo1 Lae-Hoon Kim, Raghuveer Peri, Shuhua Zhang
[Research Gate](https://www.researchgate.net/publication/333278728_Content-Based_Audio_Classification_and_Retrieval_Using_Segmentation_Feature_Extraction_and_Neural_Network_Approach) 2019
### Abstract Summary
In this paper, a feature extraction and classification methodology for audio data is proposed, where audio data are gathered in a target environment under varying conditions and corresponding features are extracted, labeled with appropriate filters (e.g., audio event descriptions), and used for training deep neural networks (DNNs) to extract underlying target audio events from unlabeled training data.

