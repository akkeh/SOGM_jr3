import numpy as np
from scipy.signal import get_window
import math
import sys, os
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../../software/models/'))
import dftModel as DFT
import utilFunctions as UF

""" 
A5-Part-1: Minimizing the frequency estimation error of a sinusoid

Write a function that estimates the frequency of a sinusoidal signal at a given time instant. The 
function should return the estimated frequency in Hz, together with the window size and the FFT 
size used in the analysis.  

The input arguments to the function are the wav file name including the path (inputFile) containing 
the sinusoidal signal, and the frequency of the sinusoid in Hz (f). The frequency of the input sinusoid  
can range between 100Hz and 2000Hz. The function should return a three element tuple of the estimated 
frequency of the sinusoid (fEst), the window size (M) and the FFT size (N) used.

The input wav file is a stationary audio signal consisting of a single sinusoid of length 1 second. 
Since the signal is stationary you can just perform the analysis in a single frame, for example in 
the middle of the sound file (time equal to .5 seconds). The analysis process would be to first select 
a fragment of the signal equal to the window size, M, centered at .5 seconds, then compute the DFT 
using the dftAnal function, and finally use the peakDetection and peakInterp functions to obtain the 
frequency value of the sinusoid.

Use a Blackman window for analysis and a magnitude threshold t = -40 dB for peak picking. The window
size and FFT size should be chosen such that the difference between the true frequency (f) and the 
estimated frequency (fEst) is less than 0.05 Hz for the entire allowed frequency range of the input 
sinusoid. The window size should be the minimum positive integer of the form 100*k + 1 (where k is a 
positive integer) for which the frequency estimation error is < 0.05 Hz. For a window size M, take the
FFT size (N) to be the smallest power of 2 larger than M. 

HINT: If the specified frequency range would have been 440-8000 Hz, the parameter values that satisfy 
the required conditions would be M = 1101, N = 2048. Note that for a different frequency range, like 
the one specified in the question, this value of M and N might not work. 

    """
def check_k(fr1, fr2, fs, k, window):
    fr = fr1
    for fr in fr1 + np.arange(fr2-fr1):
        t = np.arange(441000.0)
        x = np.sin(2.0*np.pi * fr * t / fs)
        M = 100*k+1
        i=0
        while (2**i) < M:
            i+=1
        N = 2**i
        h = M/2
        l_h = len(x)/2 - h + 1
        h_h = l_h + M
        x_cnk = x[l_h:h_h]
        w = get_window(window, M)
        (mX, pX) = DFT.dftAnal(x_cnk, w, N)
        p_loc = UF.peakDetection(mX, -40)
        p_int = UF.peakInterp(mX, pX, p_loc)
        peak = p_int[0]*(fs/float(N))
        p = peak[0]
        if abs(p-fr) > 0.05:
            print "fr: ", fr, " error: ", abs(p-fr)
            return 0
        print "fr: ", fr, " checked"
        fr+=1
        

    return 3

def findk(fr, fs, window, startk):
    k = 1 
    p=0
    while abs(p-fr) > 0.05:
        t = np.arange(441000.0)
        x = np.sin(2.0*np.pi * fr * t / fs)
        M = 100*k+1
        i=0
        while (2**i) < M:
            i+=1
        N = 2**i
        h = M/2
        l_h = len(x)/2 - h + 1
        h_h = l_h + M
        x_cnk = x[l_h:h_h]
        w = get_window(window, M)
        (mX, pX) = DFT.dftAnal(x_cnk, w, N)
        p_loc = UF.peakDetection(mX, -40)
        p_int = UF.peakInterp(mX, pX, p_loc)
        peak = p_int[0]*(fs/float(N))
        p = peak[0]
        k+=1
    return k, abs(p-fr)
def find_k(fr1, fr2, fs, window):
    maxk = 1 
    for fr in fr1+np.arange(fr2-fr1):
        k, err = findk(fr, fs, window, maxk)
        if k > maxk:
            maxk = k
        print "fr: ", fr, " k: ", k, " error: ", err
    return maxk
    
def minFreqEstErr(inputFile, f):
    """
    Inputs:
            inputFile (string) = wav file including the path
            f (float) = frequency of the sinusoid present in the input audio signal (Hz)
    Output:
            fEst (float) = Estimated frequency of the sinusoid (Hz)
            M (int) = Window size
            N (int) = FFT size
    """
    # analysis parameters:
    window = 'blackman'
    t = -40
    
    ### Your code here
    (fs, x) = UF.wavread(inputFile)
    print fs
    #k = find_k(100, 2000, fs, window)
    k=1
    
    while check_k(100, 2000, fs, k, window) < 2:
        print k
        k+=1
   
    M = 100*k+1
    print "M: ", M
    i=0
    while (2**i) < M:
        i+=1
    N = 2**i
    print "N: ", N
    print "fs: ", fs
    print "length: ", len(x)
    center = len(x)/2
    h = M/2
    x_cnk = x[center-h:center+h+1]
    print "chunk: ", len(x_cnk)
    w = get_window(window, M)
    (mX, pX) = DFT.dftAnal(x_cnk, w, N)
    p_loc = UF.peakDetection(mX, t)
    p_int = UF.peakInterp(mX, pX, p_loc)
    peak = p_int[0]*(fs/float(N))
    return (peak[0], M, N)
