// Implementation of the AudioMan library.
// Created for 3DMMForever.
// FIXME: This is a nonworking stub. The game will still function, but sound effects will be missing.

#include <Windows.h>
#include <initguid.h>

#include "audioman.h"

STDAPI AllocSoundFromStream(LPSOUND FAR *ppSound, LPSTREAM pStream, BOOL fSpooled, LPCACHECONFIG lpCacheConfig)
{
    return E_NOTIMPL;
}

STDAPI AllocSoundFromFile(LPSOUND FAR *ppSound, char FAR *szFileName, DWORD dwOffset, BOOL fSpooled,
                          LPCACHECONFIG lpCacheConfig)
{
    return E_NOTIMPL;
}

STDAPI AllocSoundFromMemory(LPSOUND FAR *ppSound, LPBYTE lpFileData, DWORD dwSize)
{
    return E_NOTIMPL;
}

STDAPI_(LPMIXER) GetAudioManMixer(void)
{
    return NULL;
}

STDAPI AllocTrimFilter(LPSOUND FAR *ppSound, LPSOUND pSoundSrc)
{
    return E_NOTIMPL;
}

STDAPI AllocBiasFilter(LPSOUND FAR *ppSound, LPSOUND pSoundSrc)
{
    return E_NOTIMPL;
}

STDAPI AllocLoopFilter(LPSOUND FAR *ppSound, LPSOUND pSoundSrc, DWORD dwLoops)
{
    return E_NOTIMPL;
}

STDAPI AllocConvertFilter(LPSOUND FAR *ppSound, LPSOUND pSoundSrc, LPWAVEFORMATEX lpwfx)
{
    return E_NOTIMPL;
}

STDAPI SoundToFileAsWave(LPSOUND pSound, char FAR *pAbsFilePath)
{
    return E_NOTIMPL;
}

STDAPI_(int) DetectLeaks(BOOL fDebugOut, BOOL fMessageBox)
{
    return 0;
}
