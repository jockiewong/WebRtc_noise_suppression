// WebRtc_official.cpp: 定义控制台应用程序的入口点。
//	用于测试 google 官方提供的 webrtc 库中的 noise suppression 模块
//	所用文件均与官方程序库中的保持一致

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "AudioFile.cpp"	
#include "AudioFile.h"	

//#include "noise_suppression.c"
//#include "ns_core.c"
//#include "defines.h"
//#include "typedefs.h"
//#include "checks.cc"

#include <windows.h>
#include "WebRtc_NS.h"
using namespace std;	
int main()
{
	AudioFile<float> af;
	// Testing wav files located in assets directory

	if (SetCurrentDirectory(TEXT("D:\\Repository\\NoiseSuppression\\WebRtc_official\\assets"))) {
		cout << "Changed current path to assets patch to load wav file." << endl;
	}
	if (!af.load("music_with_noise_16k_21db.wav")) {
		return -1;																// Cannot load wav file
	};
	af.printSummary();
	if (af.getNumChannels()>1)
	{
		for (size_t i = 0; i < af.getNumSamplesPerChannel(); i++)
		{
			af.samples[0][i] =float( 0.5*(af.samples[0][i] + af.samples[1][i]) );
		}
	}

	WebRtc_NS ns;
	ns.Init(16000, ns.kHigh);
	auto iter = af.samples[0].begin();
	float output_160[160]{ 0 };
	float input_160[160]{ 0 };
	vector<float> output;
	for (size_t i = 0; i <  af.getNumSamplesPerChannel() / 160; i++)
	{
		vector<float> data_160k_160(iter, iter + 160);
		memcpy(input_160, &data_160k_160[0], 160 * sizeof(float));
		ns.Process(input_160, output_160);
		for (size_t j = 0; j < 160; j++)
		{
			output.push_back(output_160[j]);
		}
		iter += 160;
	}
	vector<vector<float>> output_buffer{ output };
	af.setAudioBuffer(output_buffer);
	af.save("music_with_noise_16k_21db_test_nsCalss 05.wav");
    return 0;
}
