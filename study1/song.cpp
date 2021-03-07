#include "song.h"
#include<vector>
#include<iostream>
#include<string>
#include<cctype>
#include<algorithm>




void song::set_bars_init(){
	//	bar temp;
	//	temp.set_bpm(get_bpm());
	//	bars.assign(measure, temp);//各小節のbpmをセットしちゃったけどいいかどうかわからない。ソフランのときに使うのかな
	//１つ上の行のこれと下のこれ、同じのはずだけど、後者が安全に見えるから残しておく
	//	for (int i = 0; i < measure; i++){
	//		bars.push_back(temp);
	//	}
	//ポリモーフィズムの必要から、
	bars.assign(measure, NULL);
	for (std::vector<bar*>::size_type i = 0; i < bars.size(); i++){
		bars.at(i) = new bar;
//		bars.at(0)->set_bpm(bpm, 0);
	}


	/*
	bars.assign(measure, NULL);
	if (soflan_mid) {
		for (std::vector<bar*>::size_type i = 0; i < bars.size(); i++){
			bars.at(i) = new bar_sof2;
		}
		bars.at(0)->set_bpm(bpm);
	}

	else if (soflan_head){
		for (std::vector<bar*>::size_type i = 0; i < bars.size(); i++){
			bars.at(i) = new bar_sof1;
		}
		bars.at(0)->set_bpm(bpm);
	}

	else{
		for (std::vector<bar*>::size_type i = 0; i < bars.size(); i++){
			bars.at(i) = new bar;
			bars.at(i)->set_bpm(bpm);
		}
	}
	*/
}

void song::set_fumen_init(){
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++) bars.at(i)->set_fumen_init();
}

void song::input_fumen(std::string buffer){
	if (buffer.size() >= 9      && buffer.at(0) == '#'    && std::all_of(buffer.begin()+1, buffer.begin()+4, isdigit)){
		bars.at(static_cast<std::vector<bar>::size_type>(stoi(buffer.substr(1, 3))))->input_fumen(buffer.substr(4), lnobj);
	}
}

void song::convert_bpm(){
//	if (soflan_head && !(bpm_table1.empty())){
		for (std::vector<bar*>::size_type i = 0; i < bars.size(); i++){
			for (std::vector<int>::size_type j = 0; j < bars.at(i)->bpm.size(); j++){
				if (bars.at(i)->bpm.at(j) < 0){
					bars.at(i)->bpm.at(j) = get_bpm_table(bars.at(i)->bpm.at(j) * (-1));
				}
			}
		}
//	}
}

void song::complement_joint(){
//	if (soflan_head){
	//BPMの接続補完処理
	if (bars.at(0)->bpm_ttob(0) == 0) bars.at(0)->set_bpm(bpm, 0);
	for (std::vector<bar>::size_type i = 1; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		if (bars.at(i)->bpm_ttob(0) == 0) bars.at(i)->set_bpm(bars.at(i - 1)->bpm_last(), 0);
	}
//	}
	//LNOBJの接続補完補完処理
	for (int key = 0; key < 8; key++){
		for (std::vector<bar>::size_type i = 0; i < bars.size(); i++){
			for (std::vector<bool>::size_type j = 0; j < bars.at(i)->cn_swi[key].size(); j++){
				if (bars.at(i)->cn_swi[key].at(j) && bars.at(i)->cn_sus[key].at(j)){
					for (std::vector<bar>::size_type k = i; k >= 0; k--){
						std::vector<bool>::size_type l;
						if (k == i) l = j;
						else l = bars.at(k)->fumen[key].size() - 1;
						for (; l >= 0; l--){
							if (bars.at(k)->fumen[key].at(l)){
								bars.at(k)->fumen[key].at(l) = 0;
								bars.at(k)->cn_swi[key].at(l) = 1;
								break;
							}
							if (l == 0) break;
							//	if (k == 0 || l == 0) error判定部ほしいね
						}
						if (k == i && l == j) continue;
						if (bars.at(k)->cn_swi[key].at(l)) break;
					
					}
				}
			}
		}
	}
	//LNの接続補完処理
	for (int key = 0; key < 8; key++){
		bool on = 0;
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			for (std::vector<bool>::size_type j = 0; j < bars.at(i)->cn_swi[key].size(); j++){
				if(bars.at(i)->cn_swi[key].at(j)) on = !on;
				if (on) bars.at(i)->cn_sus[key].at(j) = 1;
				else bars.at(i)->cn_sus[key].at(j) = 0;
			}
		}
	}
}


//キー別ノーツ数と全体ノーツ数をオーバーロード
/*
int song::notes(int key){
	int counter = 0;
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		counter += bars.at(i).notes(key);
	}
	return counter;
}*/
/*
int song::notes(){
	int counter = 0;
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		counter += bars.at(i).notes();
	}
	return counter;
}*/
void song::read_notes(){
	int counter;
	for (int key = 0; key < 9; key++){
		counter = 0;
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			counter += bars.at(i)->get_notes(key);
		}
		notes[key] = counter;
	}
}


void song::read_time(){
	double counter = 0;
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		counter += bars.at(i)->get_time();
	}
	time = counter;
}


void song::read_density(){
	double bunshi = 0, bunbo = 0;
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		if (bars.at(i)->get_notes(9) != 0){
			bunshi += bars.at(i)->get_density() * bars.at(i)->get_time();
			bunbo += bars.at(i)->get_time();
		}
	}
	density = bunshi / bunbo;
}

void song::read_difave(){
	double ave = 0;
	double time = 0;
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		ave += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
		time += bars.at(i)->get_time();
	}
	difave = ave / time;
}

void song::read_difsdv(){
	double bunshi = 0;
	double time = 0;
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		bunshi += bars.at(i)->get_time() * pow(bars.at(i)->get_difficulty() - difave, 2);
		time += bars.at(i)->get_time();
	}
	difsdv = sqrt(bunshi / time);
}

std::string song::output_dif_list(){
	std::string str;
	double dif;
	str = "平均difficulty:\t" + std::to_string(difave) + "\r\n";
	str += "difficulty外れ値幅（両側）:\t" + std::to_string(critical * difsdv) + "\r\n";
	str += "小節\t小節難易度\t外れ値\r\n";
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		dif = bars.at(i)->get_difficulty();
		str += std::to_string(i) + "\t" + std::to_string(dif);
		if (dif > difave + critical * difsdv){
			str += "\t\t+";
		}
		if (dif < difave - critical * difsdv){
			str += "\t\t-";
		}
		str += "\r\n";
	}
	return str;
}

void song::read_difeff(){
	double bunshi = 0;
	double time = 0;
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		//ここ不安定
		if (bars.at(i)->get_difficulty() > zonewidth * (difmain - 0)){
			bunshi += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
			time += bars.at(i)->get_time();
		}
	}
	difeff = bunshi / time;
}

void song::read_difzone(){
	for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
		double dif = bars.at(i)->get_difficulty();
		std::vector<int>::size_type mes = static_cast<std::vector<int>::size_type>(dif / zonewidth + 1);
		while (difzone.size() < mes) difzone.push_back(0);
		difzone.at(mes - 1) += 1;
	}
}

std::string song::write_difzone_list(){
	std::string str;
	str = "difficultyゾーン\t小節数\tグラフ\r\n";
	for (std::vector<bar>::size_type i = 0; i < difzone.size(); i++){
		str += std::to_string(zonewidth * i) + "~" + std::to_string(zonewidth * (i + 1)) + "\t";
		str += std::to_string(difzone.at(i)) + "\t";
		for (int j = 0; j < difzone.at(i); j++) str += "|";
		str += "\r\n";
	}
	return str;
}

void song::read_difmain(){
	//ここ不安定
	std::vector<int>::size_type i = 1;//static_cast<std::vector<int>::size_type>(difave / zonewidth);
	std::vector<int>::size_type result = 0;
	int max_now = 0;
	if (i <= 0) {
		difmain = 0;  return;
	}
	for (; i < difzone.size() - 1; i++){
		//ここ不安定
		if (max_now <= difzone.at(i)){//difzone.at(i - 1) + difzone.at(i) + difzone.at(i + 1)){
			max_now = difzone.at(i);//difzone.at(i - 1) + difzone.at(i) + difzone.at(i + 1);
			result = i;
		}
	}
	difmain = result;
}