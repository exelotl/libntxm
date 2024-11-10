#include <stdio.h>
#include <nds.h>
#include <filesystem.h>

#include <ntxm/ntxm9.h>

NTXM9 *ntxm9 = 0;

int main(void)
{
	consoleDemoInit();
	nitroFSInit(NULL);

	printf("Demo XM player!\n\nLoading song...\n");

	ntxm9 = new NTXM9();
	u16 err = ntxm9->load("nitro:/test.xm");

	if(err != 0)
	{
		printf(ntxm9->getError(err));
		while(1); // Stop execution
	}
	else
		printf("Song loaded successfuly.\nPress A to play\nPress B to stop.\n\n");

	while(1)
	{
		scanKeys();
		u16 keys = keysDown();

		if(keys & KEY_A)
		{
			printf("Playing.\n");
			ntxm9->play(true);
		}
		else if(keys & KEY_B)
		{
			printf("Stopped.\n");
			ntxm9->stop();
		}

		swiWaitForVBlank();
	}

	return 0;
}
