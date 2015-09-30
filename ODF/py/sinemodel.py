import numpy as np
from scipy.signal import get_window
import sys, os
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../../software/models/'))
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../../../../../audioDSP_course/assignments/sms-tools/software/models/'))
import stft
import utilFunctions as UF
import sineModel as SM
import matplotlib.pyplot as plt


def sineODF(file='../../../../../audioDSP_course/assignments/sms-tools/sounds/piano.wav'):
    fs, x = UF.wavread(file)

    # set params:
    M = 1024    # window size
    H = int(M/3)     # hop size
    t = -80.0   #treshold (dB??)
    window = 'blackman' # window type
    fftSize = int(pow(2, np.ceil(np.log2(M))))  # size of FFT
    N = fftSize
    maxnSines = 10      # maximum simultaneous sines
    minSineDur = 0.1    # minimal duration of sines
    freqDevOffset = 30  # min(??) frequency deviation at 0Hz
    freqDevSlope = 0.001    # slope increase of min freq dev.


    w = get_window(window, M)    # get analysis window
    tStamps = genTimeStamps(len(x), M, fs, H)    # generate timestamp return?
    fTrackEst, mTrackEst, pTreckEst = SM.sineModelAnal(x, fs, w, fftSize, H, t, maxnSines, minSineDur, freqDevOffset, freqDevSlope)

    fTrackTrue = genTrueFreqTracks(tStamps) # get true freq. tracks

    # plotting:
    mX, pX = stft.stftAnal(x, fs, w, fftSize, H)
    maxplotfreq = 1500.0
    binFreq = fs*np.arange(N*maxplotfreq/fs)/N
    plt.pcolormesh(tStamps, binFreq, np.transpose(mX[:,:N*maxplotfreq/fs+1]),cmap = 'hot_r')
    # plt.plot(fTrackTrue, 'o-', color = 'c', linewidth=3.0)
    plt.plot(tStamps, fTrackEst, color = 'y', linewidth=2.0)
    # plt.legend(('True f1', 'True f2', 'Estimated f1', 'Estimated f2'))
    plt.xlabel('Time (s)')
    plt.ylabel('Frequency (Hz)')
    plt.autoscale(tight=True)
    return fTrackEst

### Do not modify this function
def genTimeStamps(xlen, M, fs, H):
    # Generates the timeStamps as needed for output
    hM1 = int(np.floor((M+1)/2))
    hM2 = int(np.floor(M/2))
    xlen = xlen + 2*hM2
    pin = hM1
    pend = xlen - hM1
    tStamps = np.arange(pin,pend,H)/float(fs)
    return tStamps

### Do not modify this function
def genTrueFreqTracks(tStamps):
    # Generates the true frequency values to compute estimation error
    # Specifically to chirp-150-190-linear.wav
    fTrack = np.zeros((len(tStamps),2))
    fTrack[:,0] = np.transpose(np.linspace(190, 190+1250, len(tStamps)))
    fTrack[:,1] = np.transpose(np.linspace(150, 150+1250, len(tStamps)))
    return fTrack

