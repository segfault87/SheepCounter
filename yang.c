/* Sheep Counter
 *
 * Voice by Han Dongseong 
 * Program by segfault and Tokigun
 *
 * Distributed under terms of the GNU General Public License v2
 */

#include <stdio.h>

/* Platform-dependent code. There is no usleep() under win32.
 * When you compile this under win32 environment, don't forget to add -DWIN32.
 */
#ifndef WIN32
#include <unistd.h>
#define SLEEP(ms) usleep(ms)
#else
#include <windows.h>
#define SLEEP(ms) SleepEx(ms < 500 ? 1 : (ms + 500) / 1000, TRUE)
#endif

#include <SDL.h>
#include <SDL_mixer.h>

const char *filenames[] = {
	"yang.wav", "1.wav", "2.wav", "3.wav", "4.wav", "5.wav", "6.wav", "7.wav",
	"8.wav", "9.wav", "10.wav", "100.wav", "1000.wav", "10000.wav", "mari.wav"
};

#ifdef DISPLAY_TEXT
const char *pronoun[] = {
	"양 ", "일", "이", "삼", "사", "오", "육", "칠", "팔", "구", "십", "백",
	"천", "만", "마리\n"
};
#endif

Mix_Chunk *audiochunks[15];

#define YANG 0
#define DIGIT(n) (n)
#define TEN 10
#define HUNDRED 11
#define THOUSAND 12
#define TENTHOUSAND 13
#define MARI 14

void speak(int n)
{
#ifdef DISPLAY_TEXT
	printf(pronoun[n]);
	fflush(stdout);
#endif
	Mix_PlayChannel(0, audiochunks[n], 0);
	while (Mix_Playing(0)) {
		SLEEP(10000);
	}
}

void speaknum(int n)
{
	int buf[20], len = 0;
	
	if (n % 10) buf[len++] = DIGIT(n % 10);
	n /= 10;
	if (n > 0) {
		buf[len++] = TEN;
		if (n % 10 > 1) buf[len++] = DIGIT(n % 10);
		n /= 10;
		if (n > 0) {
			buf[len++] = HUNDRED;
			if (n % 10 > 1) buf[len++] = DIGIT(n % 10);
			n /= 10;
			if (n > 0) {
				buf[len++] = THOUSAND;
				if (n % 10 > 1) buf[len++] = DIGIT(n % 10);
				n /= 10;
				if (n > 0) {
					buf[len++] = TENTHOUSAND;
					if (n % 10 > 1) buf[len++] = DIGIT(n % 10);
				}
			}
		}
	}
	
	speak(YANG);
	while (len--) speak(buf[len]);
	speak(MARI);
}

int main(int argc, char *argv[])
{
	int x = 1;
	int i;
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_AUDIO) == -1) {
		printf("SDL Initialization Failure: %s\n", SDL_GetError());
		return 1;
	}
	if (Mix_OpenAudio(44100, AUDIO_S16, 1, 4096)) {
		printf("Couldn't initialize audio\n");
		return 1;
	}
	Mix_AllocateChannels(1);

	/* Load audio files */
	for (i = 0; i < 15; i++) {
		audiochunks[i] = Mix_LoadWAV(filenames[i]);
		if (!audiochunks[i]) {
			printf("Couldn't open audio file: %s\n", filenames[i]);
			SDL_Quit();
			return 1;
		}
	}
	
	/* Loop */
	while (1) {
		speaknum(x);
		SLEEP(500000);
		
		++x;
	}
	
	return 0;
}

