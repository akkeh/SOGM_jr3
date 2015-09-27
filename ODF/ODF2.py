import os
import sys
import numpy as np
from scipy.signal import get_window
import matplotlib.pyplot as plt
import math

sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../../../../../audioDSP_course/assignments/sms-tools/software/models/'))
import stft
import utilFunctions as UF

eps = np.finfo(float).eps

def zeropad(x, N):
    y = np.zeros(N)
    y[:len(x)] = x
 
    return y

def nd_derivative(x, dimension):
    N = len(x)
    M = len(x[0]) 
    derv = np.ndarray(shape=(N, M), dtype='float')
    for m in range(M):
        derv[0][m] = 0;

    for n in np.arange(N - 1) + 1:
        for m in range(M):
            derv[n][m] = x[n][m] - x[n - 1][m]

    return derv

def unwrapPhase(x):
    addPIs = 0
    N = len(x)

    y = np.zeros(N)
    y[0] = x[0]
    for n in np.arange(N - 1) + 1:
        if x[n] < 0 and x[n-1] > 0:
            addPIs += 1
        y[n] = x[n] + ((np.pi+np.pi) * addPIs)
        
    return y 
        

def nd_unwrapPhase(x, dimension):
    lendim0 = len(x)      # dimension 0
    lendim1 = len(x[0])   # dimension 1

    if dimension == 0:
        N = lendim0
        M = lendim1
    else:
        N = lendim1
        M = lendim0
    
    y = np.ndarray(shape=(N, M), dtype='float')
    y = np.transpose(y)
    x = np.transpose(x)
    for m in range(M):
        y[m] = unwrapPhase(x[m])
    return np.transpose(y)

def normalise(x):
    return x / float(np.max(abs(x)))

def RMS(x, M1, M2):
    s = np.sum(x[M1:M2]*x[M1:M2])
    m = s / (M2-M1)
    r = np.sqrt(m)
    return r

def transient(x, M):
    N = len(x)
    y = np.zeros(N)

    for n in range(N):
        M1 = M2 = M/2
        
        if n < M/2:
            M1 = n
        if n > N-M/2:
            M2 = N-n
        y[n] = RMS(x, n-M1, n+M2)
    return y

def phaseFlux(inFile, window, M, fftSize, H, th, thRechargeVal, rechargeTime):
    fs, x = UF.wavread(inFile)
    x = normalise(x)
    plt.plot(-transient(x, 10))

    N = len(x)
    win = get_window(window, M)
   

    # STFT:
    X = np.ndarray(shape=(N/H, fftSize), dtype='complex')
    for n in range(N/H):
        Xpart = x[n * H:n * H + M]
        if len(Xpart) < len(win):
            Xpart = zeropad(Xpart, len(win))
        X[n] = UF.fft(Xpart * win, fftSize)    # gets auto zerophased and padded

    '''
    bins:               0   1   2   3   .. fftSize
timefrms:      
            0       [   .   .   .   .   ..  .   
            1           .   .   .   .   ..  .   
            2           .   .   .   .   ..  .
            ..          ..  ..  ..  ..  ..  ..  ]
            N/H
    '''
    
    mX = np.abs(X)  # get magnitude
    mX = normalise(mX)
    pX = np.angle(X) # get phase
    pX = normalise(pX)

    pX_uw = nd_unwrapPhase(pX, 0); # unwrapPhase
    # pX_uw = pX 

    derv1 = nd_derivative(pX_uw, 0)
    derv2 = nd_derivative(derv1, 0)

    onsets = np.zeros(N)
    onsets[0] = np.sum(derv2[0] * mX[0])
    for n in np.arange(N/H - 1) + 1:
        onsets[n*H] = np.sum(derv2[n] * (mX[n] - mX[n - 1]))
    
    '''
    # second pass?
    ODF = np.zeros(N)
    '''

    thmul = 1
    onsets = abs(normalise(onsets))
    for n in range(N/H):
        if onsets[n*H] < th * thmul:
            onsets[n*H] = 0
        else:
            thmul = thRechargeVal 
        if thmul != 1: 
            if thmul > 1:
                thmul *= rechargeTime
            else:
                thmul = 1
        
    return normalise(onsets)


