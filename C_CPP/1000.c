#include <spoj.h>
#include <cstdio>

int main()
{
	spoj_init();
	int test, sig, mem, memMax=0, tc=0;
	double score, scoreAll=0, time, timeAll=0;
	char status[4];

	while (fscanf(spoj_p_in, "%d %3s %lf %d %lf %d\n", &test, status, &score, &sig, &time, &mem)==6)
	{
		fprintf(spoj_p_info, "test %d - %s (score=%lf, sig=%d, time=%lf, mem=%d)\n", test, status, score, sig, time, mem);
		if (mem > memMax) memMax = mem;
		scoreAll += score;
		if (timeAll >= 0) {
			if (time >= 0)
				timeAll += time;
			else
				timeAll = -1;
		}
		if (status[0]!='A')
		{
			fprintf(spoj_score, "%s 0 %d %lf %d\n", status, sig, timeAll, memMax);
			return 0;
		}
		tc++;
	}
	fprintf(spoj_score, "AC %lf 0 %lf %d\n", scoreAll/tc, timeAll, memMax);
	
	return 0;
}
