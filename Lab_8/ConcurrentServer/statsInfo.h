#pragma once

struct StatsInfo
{
	long Accept;
	long Fail;
	long Finished;
	long Work;

	StatsInfo()
	{
		Accept = 0;
		Fail = 0;
		Finished = 0;
		Work = 0;
	}
};