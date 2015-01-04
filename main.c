#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <math.h>

enum corrmethod {
	setrand = 0,
	shift = 1,
	setconst = 2,
};

void showver() {
	printf(
	"putin v0.1 - versatile ROM file corruption utility\n\n"
	);
}

void showhelp() {
	printf(
	"putin v0.1 - versatile ROM file corruption utility\n\n"
	"Command line parameters:\n"
	"------------------------\n"
	"\t-i<file>    - input ROM file name\n"
	"\t-O<file>    - output ROM file name\n"
	"\t-o<bytes>   - offset in bytes\n"
	"\t-s<seed>    - random seed to use\n"
	"\t-e<exec>    - program (emulator) to execute with output file as parameter\n"
	"\t-m<method>  - corruption method: 0 - set random bytes, 1 - add value to bytes, 2 - set bytes to value\n"
	"\t-c<bytes>   - size of area to corrupt\n"
	"\t-p<prob>    - probability that a byte in area will be modified\n"
	"\t-v<value>   - value to set bytes to or shift bytes by\n"
	"\t-h or -?    - show this help message\n"
	"\t-V          - show version info\n"
	);
}

int main(int argc, char** argv)
{
	char* infile = NULL;
	char* outfile = NULL;
	char* emulator = NULL;
	enum corrmethod method = 0;
	uint8_t prob = 15; // (n+1) out of 256.
	uint8_t value = 0; //shift by what, set to what?
	uint32_t offset = 0; //method from where?
	uint32_t count = 256; //how many bytes to method?
	uint32_t seed = time(NULL); //seed
	int i;

	/*
	for (i=1;i<argc;i++) {
		if (strncmp(argv[i],"-O",2) == 0) outfile = (argv[i])+2; else
		if (strncmp(argv[i],"-o",2) == 0) sscanf(argv[i],"-o%i",&offset); else
		if (strncmp(argv[i],"-s",2) == 0) sscanf(argv[i],"-s%d",&seed); else
		if (strncmp(argv[i],"-e",2) == 0) emulator = (argv[i])+2; else
		if (strncmp(argv[i],"-m",2) == 0) sscanf(argv[i],"-m%d",&method); else
		if (strncmp(argv[i],"-c",2) == 0) sscanf(argv[i],"-c%i",&count); else
		if (strncmp(argv[i],"-p",2) == 0) {sscanf(argv[i],"-p%d",&prob); prob--;} else
		if (strncmp(argv[i],"-v",2) == 0) sscanf(argv[i],"-v%i",&value); else
		infile = (argv[i]);
	}
	*/

	int opt;

	while ((opt = getopt(argc, argv, ":i:O:o:s:e:m:c:p:v:hV")) != -1) {
		switch (opt) {
			case 'i':
				infile = optarg; break;
			case 'O':
				outfile = optarg; break;
			case 'o':
				offset = strtol(optarg,NULL,0); break;
			case 's':
				seed = strtol(optarg,NULL,0); break;
			case 'e':
				emulator = optarg; break;
			case 'm':
				method = strtol(optarg,NULL,0); break;
			case 'c':
				count = strtol(optarg,NULL,0); break;
			case 'p':
				prob = (strtol(optarg,NULL,0) - 1); break;
			case 'v':
				value = strtol(optarg,NULL,0); break;
			case 'h':
			case '?':
				showhelp(); exit(0); break;
			case 'V':
				showver(); exit(0); break;
			case ':':
				printf("Unknown key -%c or key requires a parameter\n",optopt); exit(1); break;
			default: 
				printf("getopt wtf -%c\n",optopt); exit(1); break;
		}
	}
	
	srandom(seed);

	if (prob < 0) prob=0;

	if (infile == NULL) { printf("Input file not specified\n."); return 1;}
	if (outfile == NULL) {
		outfile = malloc(strlen(infile) + 2);
		strcpy(outfile,"_");
		strcat(outfile,infile);
	}
	printf("Input file: %s\nOutput file: %s\nmethodion method: %d\nProbability: %d/256\nValue: %d\nOffset:%#x\nRandom seed:%d\n",infile,outfile,method,prob+1,value,offset,seed);
	printf("Emulator: %s\nArea to corrupt: %dB\n",emulator,count);

	FILE* in = fopen(infile,"rb");
	if (in == NULL) { perror("Unable to open input file"); return 1;}

	FILE* out = fopen(outfile,"wb");	
	if (out == NULL) { perror("Unable to create output file"); return 1;}

	int fsz = -1;
	fseek(in,0L,SEEK_END);
	fsz = ftell(in);
	fseek(in,0L,SEEK_SET);	

	if ((offset > fsz) || ((offset + count) > fsz)) printf("File offset or count too large. File size is %d bytes.\n",fsz);

	char* data = malloc(offset);
	fread(data,offset,1,in);
	fwrite(data,offset,1,out);
	free(data);
	data = malloc(count);
	fread(data,count,1,in);
	for (i=0;i<count;i++)
	{
		if ((random() % 256) <= prob) {
			switch (method) {
				case setrand:
					data[i] = (char)random();
					break;
				case shift:
					data[i] += value;
					break;
				case setconst:
					data[i] = value;
					break;
			}
		}
	}
	fwrite(data,count,1,out);
	free(data);
	data = malloc(fsz-offset-count);
	fread(data,fsz-offset-count,1,in);
	fwrite(data,fsz-offset-count,1,out);
	free(data);
	fclose(in);
	fclose(out);

	if (emulator != NULL) {
		char* cmd = malloc(strlen(emulator) + strlen(outfile) + 4);
		strcpy(cmd,emulator);
		strcat(cmd," \"");
		strcat(cmd,outfile);
		strcat(cmd,"\"");
		printf("calling %s\n",cmd);

		system(cmd);
	}

	return 0;
}
