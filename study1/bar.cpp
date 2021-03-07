#include "bar.h"
#include<string>
#include<iostream>
#include<vector>

using namespace std;


int fractional(int n){
	if (n <= 1){
		return 1;
	}
	return n * fractional(n - 1);
}


//譜面を初期化。小節長のboolを各鍵盤に用意
void bar::set_fumen_init(){
	vector<bool> temp(resolution * beat, 0);//int resolution * float beatでintに落としている暗黙のキャスト。
	for (std::vector<bool>::size_type i = 0; i < 8; i++){
		fumen[i] = temp;
		cn_swi[i] = temp;
		cn_sus[i] = temp;
	}
}


//譜面取り込み用関数
void bar::input_fumen(std::string buffer, std::string lnobj){
	int channel = std::stoi(buffer.substr(0, 2));
	std::string::size_type length = (buffer.size() - 3) / 2;
	std::vector<bool>::size_type interval = fumen[0].size() / static_cast<std::vector<bool>::size_type>(length);


	std::string::size_type i = 0;
	std::vector<bool>::size_type tick = 0;
	for (; i < length; tick += interval, i++){
		if (lnobj.size() == 2 && buffer.at(3 + i * 2) == lnobj.at(0) && buffer.at(4 + i * 2) == lnobj.at(1)){
			switch (channel){
			case 11:cn_swi[1].at(tick) = 1; cn_sus[1].at(tick) = 1; break;
			case 12:cn_swi[2].at(tick) = 1; cn_sus[2].at(tick) = 1; break;
			case 13:cn_swi[3].at(tick) = 1; cn_sus[3].at(tick) = 1; break;
			case 14:cn_swi[4].at(tick) = 1; cn_sus[4].at(tick) = 1; break;
			case 15:cn_swi[5].at(tick) = 1; cn_sus[5].at(tick) = 1; break;
			case 18:cn_swi[6].at(tick) = 1; cn_sus[6].at(tick) = 1; break;
			case 19:cn_swi[7].at(tick) = 1; cn_sus[7].at(tick) = 1; break;
			case 16:cn_swi[0].at(tick) = 1; cn_sus[0].at(tick) = 1; break;
			default:break;
			}
		}
		else if (buffer.at(3 + i * 2) != '0' || buffer.at(4 + i * 2) != '0'){
			switch (channel){
			case 11:fumen[1].at(tick) = 1; break;
			case 12:fumen[2].at(tick) = 1; break;
			case 13:fumen[3].at(tick) = 1; break;
			case 14:fumen[4].at(tick) = 1; break;
			case 15:fumen[5].at(tick) = 1; break;
			case 18:fumen[6].at(tick) = 1; break;
			case 19:fumen[7].at(tick) = 1; break;
			case 16:fumen[0].at(tick) = 1; break;
			case  3:{
				bpm_tick.push_back(tick);
				bpm.push_back(stoi(buffer.substr(3 + i * 2, 2), nullptr, 16));
				break;
			}
			case  8:{
				bpm_tick.push_back(tick);
				bpm.push_back(stoi(buffer.substr(3 + i * 2, 2), nullptr, 16) * (-1));
				break;
			}
			case 51:cn_swi[1].at(tick) = 1;  break;
			case 52:cn_swi[2].at(tick) = 1;  break;
			case 53:cn_swi[3].at(tick) = 1;  break;
			case 54:cn_swi[4].at(tick) = 1;  break;
			case 55:cn_swi[5].at(tick) = 1;  break;
			case 58:cn_swi[6].at(tick) = 1;  break;
			case 59:cn_swi[7].at(tick) = 1;  break;
			case 56:cn_swi[0].at(tick) = 1;  break;
			default:break;
			}
		}
	}
}


void bar::read_time(){
	double sumtime = 0.0;
	int tick = bpm_tick.at(0);
	std::vector<int>::size_type i = 1;
	for (; i < bpm.size(); i++){
		sumtime += 1.0 / bpm.at(i - 1) * 60 * (bpm_tick.at(i) - bpm_tick.at(i - 1)) / resolution * 4;
	}
	sumtime += 1.0 / bpm.at(i - 1) * 60 * (beat * resolution - bpm_tick.at(i - 1)) / resolution * 4;
	time = sumtime;
}


void bar::read_notes(){
	int counter;
	for (int key = 0; key < 8; key++){
		counter = 0;
		for (std::vector<bool>::size_type tick = 0; tick < fumen[key].size(); tick++){
			if (fumen[key].at(tick)){
				counter++;
			}
			if (cn_swi[key].at(tick) && cn_sus[key].at(tick)){
				counter++;
			}
		}
		notes[key] = counter;
	}
	notes[8] = 0;
	for (int key = 0; key < 8; key++){
		notes[8] += notes[key];
	}
}


//全部改善の余地あり
void bar::read_difficulty(){
	double add = 0.0;

	//乱打偏り
	for (std::vector<bool>::size_type i = 0; i < fumen[0].size(); i++){
		double douji = 0.0;
		for (int k = 0; k < 8; k++){
			if (fumen[k].at(i))
				douji += 1;
		}
		if(douji) add += (douji - 1) / 7;
	}
	//皿
	add += sqrt(notes[0]);
	//軸（トリル、折り返し）
	//縦連
	for (int key = 1; key < 8; key++){
		std::vector<bool>::size_type interval = 0;
		double time = 0.0;
		int bpm = bpm_ttob(0);
		int bank = 0;
		double tateren_threshold = 1.0 * 24 * 60 * 4 / 110 / resolution;
		double jiku_threshold = 1.0 * 48 * 60 * 4 / 110 / resolution;
		for (std::vector<bool>::size_type i = 0; i < fumen[0].size(); i++){
			if (bpm_ttob(i)){
				time += 1.0 * interval * 60 * 4 / bpm / resolution;//[sec]
				bpm = bpm_ttob(i);
				interval = 0;
			}
			if (fumen[key].at(i)){
				time += 1.0 * interval * 60 * 4 / bpm / resolution;
				if (time < tateren_threshold){
					bank += 1;
				}
				else if (time < jiku_threshold){
					if(bank) add += 0.5;
					bank = 1;
				}
				else{
					if (bank == 2)
						add += 0.5;
					if (bank > 2)
						add += bank;
					bank = 1;
				}
				interval = 0;
				time = 0.0;
			}
			else interval++;
		}
		if (bank == 2)
			add += 0.5;
		if (bank > 2)
			add += bank;
	}
	//CN複合、開始点ゴミつきボーナスもつける
	for (std::vector<bool>::size_type i = 0; i < fumen[0].size(); i++){
		if ((cn_sus[1].at(i) || cn_sus[2].at(i) || cn_sus[3].at(i) || cn_sus[4].at(i) || cn_sus[5].at(i) || cn_sus[6].at(i) || cn_sus[7].at(i))
			&& (fumen[1].at(i) || fumen[2].at(i) || fumen[3].at(i) || fumen[4].at(i) || fumen[5].at(i) || fumen[6].at(i) || fumen[7].at(i))){
			add += 1.0;
		}
		//CNカーテン
		int off = 0;
		int sus = 0;
		int on = 0;
		for (int k = 1; k < 8; k++){
			if (cn_swi[k].at(i) && cn_sus[k].at(i) == 0) off++;
			if (cn_swi[k].at(i) == 0 && cn_sus[k].at(i)) sus++;
			if (cn_swi[k].at(i) && cn_sus[k].at(i)) on++;
		}
		if (off){
			if (sus > 0){
				add += fractional(off + on + sus) / fractional(on + off) / fractional(sus);
			}
			else add += off;
		}
		else if (sus && on){
			add += on;
		}





		//BSS複合
		if (cn_sus[0].at(i)){
			for (int k = 1; k < 8; k++){
				if (fumen[k].at(i)) add += 1.0;
				if (cn_swi[k].at(i) && cn_sus[k].at(i)) add += 1.0;
				if (cn_sus[k].at(i) && fumen[k].at(i)) add += 1.0;
			}
		}


	}
	difficulty = density + add / time;
//	difficulty = density;

}



/*派生の名残
void bar_sof1::input_fumen(std::string buffer){
	int channel = std::stoi(buffer.substr(0, 2));
	std::string::size_type length = (buffer.size() - 3) / 2;
	std::vector<bool>::size_type interval = fumen[0].size() / static_cast<std::vector<bool>::size_type>(length);

	if (channel == 3){
		bpm = stoi(buffer.substr(3), nullptr, 16);
		return;
	}
	if (channel == 8){
		bpm = stoi(buffer.substr(3), nullptr, 16) * (-1);
		return;
	}

	std::string::size_type i = 0;
	std::vector<bool>::size_type tick = 0;
	for (; i < length; tick += interval, i++){
		if (buffer.at(3 + i * 2) != '0' || buffer.at(4 + i * 2) != '0'){
			switch (channel){
			case 11:fumen[1].at(tick) = 1; break;
			case 12:fumen[2].at(tick) = 1; break;
			case 13:fumen[3].at(tick) = 1; break;
			case 14:fumen[4].at(tick) = 1; break;
			case 15:fumen[5].at(tick) = 1; break;
			case 18:fumen[6].at(tick) = 1; break;
			case 19:fumen[7].at(tick) = 1; break;
			case 16:fumen[0].at(tick) = 1; break;
			default:break;
			}
		}
	}
}



void bar_sof2::input_fumen(std::string buffer){
	int channel = std::stoi(buffer.substr(0, 2));
	std::string::size_type length = (buffer.size() - 3) / 2;
	std::vector<bool>::size_type interval = fumen[0].size() / static_cast<std::vector<bool>::size_type>(length);


	std::string::size_type i = 0;
	std::vector<bool>::size_type tick = 0;
	for (; i < length; tick += interval, i++){
		if (buffer.at(3 + i * 2) != '0' || buffer.at(4 + i * 2) != '0'){
			switch (channel){
			case 11:fumen[1].at(tick) = 1; break;
			case 12:fumen[2].at(tick) = 1; break;
			case 13:fumen[3].at(tick) = 1; break;
			case 14:fumen[4].at(tick) = 1; break;
			case 15:fumen[5].at(tick) = 1; break;
			case 18:fumen[6].at(tick) = 1; break;
			case 19:fumen[7].at(tick) = 1; break;
			case 16:fumen[0].at(tick) = 1; break;
			case  3:{bpm_tick.push_back(tick);
				bpm_value.push_back(stoi(buffer.substr(3 + i * 2, 2), nullptr, 16));
			case  8:{bpm_tick.push_back(tick);
				bpm_value.push_back(stoi(buffer.substr(3 + i * 2, 2), nullptr, 16) * (-1)); 
			}
			default:break;
			}
		}
	}
}
*/