#!/usr/bin/python3

# voice processing, file manipulation, etc
from vosk import Model, KaldiRecognizer, SetLogLevel
import sys
import os
import wave
import subprocess
import json
# the essentials :)
from scipy import signal
import matplotlib.pyplot as plt
import math as math
import numpy as np
# pyserial, logging, etc
import serial
import serial.tools.list_ports
import glob
import platform
from datetime import datetime

###############################################
#  Converting VOX encoded data to 16 bit PCM  #
###############################################

# VOX decription is adapted from code written by IvanEvan on Github
# https://github.com/IvanEvan/Vox2Wav-by-python

# table of quantizer step size
StepSizeTable = [16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41,
                 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173,
                 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658,
                 724, 796, 876, 963, 1060, 1166, 1282, 1408, 1552]

# another conversion table
IndexTable = [-1, -1, -1, -1, 2, 4, 6, 8]

def ADPCM_Encode(sample):
    global index
    global predsample

    code = 0

    step_size = StepSizeTable[index]

    # compute diff and record sign and absolut value
    diff = sample - predsample
    if diff < 0:
        code = 8
        diff = -diff

    # quantize the diff into ADPCM code
    # inverse quantize the code into a predicted diff
    tmpstep = step_size
    diffq = step_size >> 3

    if diff >= tmpstep:
        code = code | 0x04
        diff -= tmpstep
        diffq = diffq + step_size

    tmpstep = tmpstep >> 1

    if diff >= tmpstep:
        code = code | 0x02
        diff = diff - tmpstep
        diffq = diffq + (step_size >> 1)

    tmpstep = tmpstep >> 1

    if diff >= tmpstep:
        code = code | 0x01
        diffq = diffq + (step_size >> 2)

    # fixed predictor to get new predicted sample
    if code & 8:
        predsample = predsample - diffq
    else:
        predsample = predsample + diffq

    # check for overflow
    if predsample > 32767:
        predsample = 32767
    elif predsample < -32768:
        predsample = -32768

    # find new stepsize index
    index += IndexTable[code]

    # check for overflow
    if index < 0:
        index = 0

    if index > 48:
        index = 48

    # return new ADPCM code   code & 0x0f == code
    return code & 0x0f


# ADPCM_Decode.
# code: a byte containing a 4-bit ADPCM sample.
# retval : 16-bit ADPCM sample
de_index = 0
de_predsample = 0

def ADPCM_Decode(code):
    global de_index
    global de_predsample

    step_size = StepSizeTable[de_index]

    # inverse code into diff
    diffq = step_size >> 3  # == step/8
    if code & 4:
        diffq += step_size

    if code & 2:
        diffq += step_size >> 1

    if code & 1:
        diffq += step_size >> 2

    # add diff to predicted sample
    if code & 8:
        diffq = -diffq

    de_predsample += diffq

    # check for overflow  clip the values to +/- 2^11 (supposed to be 16 bits)
    if de_predsample > 2047:
        de_predsample = 2047
    elif de_predsample < -2048:
        de_predsample = -2048

    # find new quantizer step size
    de_index += IndexTable[code]

    # check for overflow
    if de_index < 0:
        de_index = 0

    if de_index > 48:
        de_index = 48

    # save predict sample and de_index for next iteration
    # return new decoded sample
    # The original algorithm turned out to be 12bit, need to convert to 16bit
    return de_predsample << 4


# this function converts 8 bit VOX APCDM encrypted data to a raw 16 bit PCM
def decodeTBS2(list_8bit):
    list_16bit = []
    for i in range(len(list_8bit)):
        byte_i = list_8bit[i]  # 1 bytes = 8bit
        high_4bit = (byte_i & 0xf0) >> 4  # split high 4bit from 8bit
        low_4bit = byte_i & 0x0f  # split low 4bit from 8bit

        # first sample
        sample_0 = high_4bit
        # unsigned to signed
        # 4bit : -2^4 ~ 2^(4-1)-1
        if sample_0 > 7:
            sample_4bit_0 = sample_0 - 16
        else:
            sample_4bit_0 = sample_0

        # second sample
        sample_1 = low_4bit
        # unsigned to signed
        if sample_1 > 7:
            sample_4bit_1 = sample_1 - 16
        else:
            sample_4bit_1 = sample_1

        # now decode
        tmpDeS16_0 = ADPCM_Decode(sample_4bit_0)
        tmpDeS16_1 = ADPCM_Decode(sample_4bit_1)

        list_16bit.extend([tmpDeS16_0, tmpDeS16_1])

    # decoded data
    return list_16bit

# Converts a 16 bit numpy array to a .wav file
# list_16bit - raw 16 bit PCM data
# name - name of .wav file (don't include .wav
def toWav(list_16bit, name):
    wav_file = wave.open(name + '.wav', 'wb')
    # configure channel number, quantization size, and sample rate
    wav_file.setnchannels(1)
    wav_file.setsampwidth(2)
    wav_file.setframerate(16000)
    # converts data to binary data and writes it to a file
    wav_file.writeframes(list_16bit.tobytes())
    wav_file.close()

#########################################################   
#  functions for converting .wav to strings using Vosk  #
#########################################################


# convert audio from a wavefile e.g. "example.wav" to 
def wav2str(filename, sample_rate = 16000, foldername = "voskmodel"):
    # this is the name of the model folder
    model = Model(foldername)
    rec = KaldiRecognizer(model, sample_rate)

    wf = wave.open(filename, "rb")
    if wf.getnchannels() != 1 or wf.getsampwidth() != 2 or wf.getcomptype() != "NONE":
        print ("Audio file must be WAV format mono PCM.")
        exit (1)

    results = []
    subs    = []
    while True:
        data = wf.readframes(4000)
        if len(data) == 0:
            break
        if rec.AcceptWaveform(data):
            results.append(rec.Result())
    results.append(rec.FinalResult())

    Strings = []
    for i, res in enumerate(results):
        jres = json.loads(res)
        if not 'result' in jres:
            continue
        words = jres['result']
        for j in range(len(words)):
            Strings.append(words[j]['word'])
    return Strings

#######################################
#  Methods of comparing Vosk results  #
#######################################

# to compare two lists, regular edit distance...
def edit_dist(A, B):
    if len(A) <= len(B):
        shorter,longer = A,B
    else:
        shorter,longer = B,A

    a = np.zeros((2,len(shorter) + 1), dtype=int) # matrix of values

    # get the first row
    for i in range(len(shorter)+1):
        a[0][i] = i                      # 0th row

    # get the rest of the rows
    for j in range(1,len(longer)+1):
        a[1][0] = j                          # first column
        for i in range(1,len(shorter)+1):
            a[1][i] = min([a[0][i-1] + (longer[j-1] != shorter[i-1]),
                           a[0][i] + 1,
                           a[1][i-1] + 1])
        a[0] = a[1]                          # push row back

    return(a[0][len(shorter)])           # return last value

# a method for comparing two lists of strings
# there are other ways this could be implemented
# this is just one way that worked
# normalizes to [0,1] so that it's easier to compare strings
def list_compare(L1,L2):
    if not len(L1) and not len(L2):
        return 0

    D = np.zeros((len(L1)+1,len(L2)+1))
    for i in range(len(L1)):
        D[i+1,0] = i+1
    for j in range(len(L2)):
        D[0,j+1] = j+1

    for i in range(1,len(L1)+1):
        for j in range(1,len(L2)+1):
            D[i,j] = min([D[i-1][j-1] + edit_dist(L1[i-1],L2[j-1])/max(len(L1[i-1]),len(L2[j-1])),
                           D[i-1][j] + 1,
                           D[i][j-1] + 1])
    return D[len(L1),len(L2)]/max(len(L1),len(L2))

# returns -1 for error otherwise returns index of input
# Input - input command to be interpreted
# Commands - list of available commands through vosk results
# Threshold - similarity required to be acceptable
def select_command(text, commands, threshold = 0.4):
    argMax, Max = -1, 0
    for i in range(len(commands)):
        Similarity = 0
        for j in range(len(commands[i])):
            Similarity = Similarity + (1-list_compare(text, commands[i][j]))
        Similarity = Similarity / len(commands[i])
        if Similarity > Max and Similarity > threshold:
            argMax, Max = i, Similarity
    return argMax

# CMD1: 'test command one'
cmd1 = [
  ['test', 'command', 'one'],
  ['command', 'one'],
  ['test', 'command', 'one'],
  ['test', 'command', 'one'],
  ['that\'s', 'command', 'one']
]
# CMD2: 'some long sentence'
cmd2 = [
  ['some', 'one', 'sentence'],
  ['some', 'sentence'],
  ['the', 'sentence'],
  ['some', 'long', 'sentence'],
  ['some', 'sentence']
]
# CMD3: 'dog' (corner case short command)
cmd3 = [
  ['dog'],
  ['dog'],
  ['dog'],
  [],
  ['talk']
]
# CMD4: 'test command four' (corner case similar commands)
cmd4 = [
  ['test', 'command'],
  ['command', 'for'],
  ['test', 'command', 'for'],
  ['test', 'command', 'for'],
  ['test', 'command', 'for']
]
# cmds: list of all command lists
#   these commands are pre-trained to save time during the demo
cmds = [cmd1, cmd2, cmd3, cmd4]
cmds_map = {0: 'UNREC AUDIO CMD', 1: 'REMT1 NODE1 ON', 2: 'REMT1 NODE1 OFF', 
            3: 'REMT1 NODE1 TOG', 4: 'REMT1 NODE2 TOG'}

#####################
#  Serial Commands  #
#####################

def printLog(*args, **kwargs):
    stamp = str(datetime.now())
    print('[' + stamp + ']', *args)

def errorLog(*args, **kwargs):
    stamp = str(datetime.now())
    print('[' + stamp + ']', *args)

def listPorts():
    devOS = platform.system()
    printLog('Detected', devOS, 'Computer')
    ports = []
    if devOS == 'Windows':
        ports = serial.tools.list_ports.comports()
    elif devOS == 'Linux':
        ports = glob.glob('/dev/tty[A-Za-z]*')
    printLog('Listing all Serial Ports:')
    for p in ports:
        printLog(p)

def setupSerial():
    listPorts()
    while True:
        try:
            port = input('\tenter port: ')
            s = serial.Serial(port)
            s.baudrate = 115200
            s.bytesize = serial.EIGHTBITS
            s.parity = serial.PARITY_NONE
            s.stopbits = serial.STOPBITS_ONE
            printLog('Serial Port open on', port.upper(), 'for EFR32MG12')
            return s
        except serial.SerialException:
            errorLog('Invalid Serial Port, please try again!')
            continue

################
#  Main Loop   #
################

def main(timeout):
    ser = setupSerial()

    while(True):
        # read and process from serial
        res = ser.readline()
        share = False

        # audio data
        if b'st' in res:
            # read until BTN timeout
            ser.timeout = timeout
            printLog('Reading from Serial Port!')
            res = ser.read(16000*timeout)

            # process data from Vox to PCM
            printLog('Converting VOX to PCM')
            RawPCM = np.array(decodeTBS2(res),dtype=np.int16)[1500:]

            # save as wav file with
            printLog('Creating .WAV File to Parse')
            filename = 'input_wav'
            toWav(RawPCM, filename)

            # gather list of strings from Vox
            printLog('Converting Audio to Text')
            parsed_text = wav2str(filename + '.wav')
            printLog('Parsed Text:\n\t', parsed_text)

            # anticipated command by comparing to other vosk results
            printLog('Matching Text against Commands')
            result = '-1\n'
            if parsed_text:
                index = select_command(parsed_text, cmds) + 1
                if index > 0:
                    result = str(index) + '\n'
            printLog('Matched to CMD#', result)
            res = "AUDIO " + cmds_map[index]
            share = True
            ser.timeout = 0
        
        else:
            res = res.decode().strip()

        if share:
            printLog('INP: ', res)
            fname = 'share/p2n/' + datetime.now().strftime("%Y%m%d-%H%M%S") + '.txt'
            fnew = open(fname, 'w')
            fnew.write(res)
            fnew.close()

        # read files if any and send to serial
        dir = 'share/n2p/'
        for fname in os.listdir(dir):
            print("filename: " + fname)
            fp = open(dir+fname)
            dt = fp.read()
            print("data: " + dt)
            ser.write(dt)
            fp.close()
            os.remove(dir+fname)

if __name__ == '__main__':
    main(timeout = 5)
