//executeの定義ファイル。譜面のパスをもらって、ファイルオープンしてクラスsongに譜面を渡し、クラスsongから分析結果をもらって出力文章を作る関数。

#include "MyForm.h"
#include"bar.h"
#include"song.h"

#include<string>
#include<iostream>
#include<fstream>
#include<cctype>
#include<algorithm>

using namespace std;

//グローバル変数、マジックナンバーはここ
const int resolution = 384;//譜面縦解像度
const double critical = 2.0;//はずれ検定、output_def_list
const double effect = 1.0;//difeff
const double zonewidth = 0.5;//difficult離散階層化の幅

string study1::MyForm::execute(const char* filename){

	string result;
	string buffer;
	string str;
	string::size_type i;
	vector<bar>::size_type j;
	int k;

	//bmsファイルをオープン
	ifstream fis(filename);
	if (!fis.is_open()) {
		result = "ファイルが開けませんでした。";
		return result;
	}

	//曲格納クラスを作る
	song songs;
	result = "結果\r\n\r\n";

	//bmsファイル走査１
	//まずはsongのbpmと小節数measureを取得。取得できるかどうかをbmsファイルの要件とする。
	while (!fis.eof()){
		getline(fis, buffer);

		//bmsファイルからbpmを取得してsong.bpmに入れる。（行頭）#BPM（任意数スペース）***の形
		if (buffer.find("#BPM ") == 0){
			i = 0;
//			while ( buffer.at(i) < '0' || '9' < buffer.at(i))i++;
			songs.set_bpm(stoi(buffer.substr(5)));
		}
		//bmsファイルから小節数を取得してsong.measureに入れる。（行頭）#aaabbの形のaaaの最大値を取得
		if (buffer.size() >= 9 && buffer.at(0) == '#'    && std::all_of(buffer.begin() + 1, buffer.begin() + 4, isdigit)){
			if (songs.get_measure() < stoi(buffer.substr(1, 3))) songs.set_measure(stoi(buffer.substr(1, 3)));
			//小節頭でBPM変化をするソフランがあったらフラグを立てる。頭じゃなかったらそっちのフラグを立てる。
			if (buffer.at(4) == '0' && (buffer.at(5) == '3' || buffer.at(5) == '8')){
				if (buffer.size() == 9){
					songs.soflan_head_on();
				}
				else{
					songs.soflan_mid_on();
				}
			}
		}
		//シングルプレイかを確認
		if (buffer.find("#PLAYER") == 0){
			if (buffer.substr(8) != "1") {
				result = "現状シングルプレイしか扱ってません\r\n";
				return result;
			}
		}
		//タイトルを取得
		if (buffer.find("#TITLE") == 0){
			if (buffer.size() >= 8)	result += "title:\t" + buffer.substr(7) + "\r\n";
			else result += "title:\t[none]\r\n";
		}
		//LNOBJを取得
		if (buffer.find("#LNOBJ") == 0){
			if (buffer.size() >= 9) songs.set_lnobj(buffer.substr(7));
		}
	}
	//bms要件の検査。bpmとmeasureが0でない。
	if (songs.get_bpm() == 0){
		result = "bpmが取得できませんでした。期待される形は、#BPM ***です。\r\n";
		return result;
	}
	if (songs.get_measure() == 0){
		result = "小節数が取得できませんでした。期待される形は、#aaabbです。\r\n";
		return result;
	}
	//小節数measureは、000~max(aaa)のaaa+1つ。
	else songs.set_measure(songs.get_measure() + 1);


	//songs.barsを初期化（measure依存）barの種類を決定している。
	songs.set_bars_init();

	//bmsファイル走査２
	//各小節の拍子をsongs.bars[].beatに取得。ついでに#difficultyも
	fis.clear();
	fis.seekg(0, ios_base::beg);
	while (!fis.eof()){
		getline(fis, buffer);
		if (buffer.size() >= 9 && buffer.at(0) == '#'    && std::all_of(buffer.begin() + 1, buffer.begin() + 4, isdigit) && buffer.at(4) == '0' && buffer.at(5) == '2'){
			songs.bars.at(static_cast<std::vector<bar>::size_type>(stoi(buffer.substr(1, 3))))->set_beat(stof(buffer.substr(7)));
		}
		//		if (buffer.find("DIFFICULTY")){} めんどくさくなったｗ
		//#bpmbb xxxの対応表を作る。
		if (buffer.find("#BPM") == 0 && buffer.at(4) != ' ') songs.set_bpm_table(stoi(buffer.substr(4, 2), nullptr, 16), stoi(buffer.substr(7)));
	}

	//songs.bars[].fumenを初期化（beat依存）
	songs.set_fumen_init();


	//bmsファイル走査３
	//譜面を取り込み。実際の動作はクラスに任せ、ここではファイルから一行取得してクラスに渡すだけ。
	fis.clear();
	fis.seekg(0, ios_base::beg);
	while (!fis.eof()){
		getline(fis, buffer);
		songs.input_fumen(buffer);
	}
	fis.clear();
	//bpmの変換
	songs.convert_bpm();
	//小節またぎが必要な、ソフランとLNの処理
	songs.complement_joint();

	//全解析値をクラスに計算させ、クラスのメンバ変数に格納
	songs.read_all();

	//表示
	result += "bpm:\t" + to_string(songs.get_bpm()) + "\r\n";
	result += "小節数:\t" + to_string(songs.get_measure()) + "\r\n";
	result += "時間:\t" + to_string(songs.get_time()) + "[秒]\r\n";
	result += "おすすめTOTAL値:\t" + to_string(static_cast<int>((7.605 * songs.get_notes(8)) / (0.01 * songs.get_notes(8) + 6.5))) + "\r\n";
	result += "\r\n";
	result += "総ノーツ数:\t" + to_string(songs.get_notes(8)) + " (LNは始点のみ数える)\r\n";
	result += "平均密度\t" + to_string(songs.get_density()) + "[notes/秒]\r\n";
	for (int key = 1; key < 8; key++){
		result += to_string(key) + "鍵:\t" + to_string(songs.get_notes(key)) + "\t";
		if (static_cast<double>(songs.get_notes(key)) / songs.get_notes(8) < 0.1)result += "0";
		result += to_string(static_cast<double>(songs.get_notes(key)) / songs.get_notes(8) * 100) + "%\r\n";
	}
	result += "ｵ皿:\t" + to_string(songs.get_notes(0)) + "\t";
	if (static_cast<double>(songs.get_notes(0)) / songs.get_notes(8) < 0.1)result += "0";
	result += to_string(static_cast<double>(songs.get_notes(0)) / songs.get_notes(8) * 100) + "%\r\n";
	result += "\r\n";
	result += "小節\t小節難度\tノーツ数\tグラフ（10刻み）\r\n";
	for (j = 0; j < static_cast<std::vector<bar>::size_type>(songs.get_measure()); j++){
		result += to_string(j) + "\t" + to_string(songs.bars.at(j)->get_difficulty()) + "\t" + to_string(songs.bars.at(j)->get_notes(8)) + "\t";
		for (k = 0; k < songs.bars.at(j)->get_notes(8); k++){
			result += "|";
			if (k % 10 == 9) result += " ";
		}
		result += "\r\n";//これクラス内に定義したほうがいい？
	}//クラスの内側にdraw_notes_graphをつくるべきかもしれない。
	result += "\r\n";
	result += songs.write_difzone_list();
	result += "\r\n";
	result += "難易度平均\t" + to_string(songs.get_difave()) + "\r\n";
	result += "難易度標準偏差\t" + to_string(songs.get_difsdv()) + "\r\n";
	result += "有効難易度:\t" + to_string(songs.get_difeff()) + "\r\n";
	//ここ不安定
	result += "メイン難易度帯\t" + to_string(zonewidth * (songs.get_difmain() - 0)) + "~" + to_string(zonewidth * (songs.get_difmain() + 1)) + "\r\n";


	//デバッグ用
//	for (int k = 0; k < 4; k++) result += to_string(songs.bars[k].aaaaa()) + "\r\n";

//	for (std::vector<bool>::size_type k = 0; k < 384; k++)result += to_string(songs.bars[9].aaaa(k));

//	result += to_string(songs.bars[1].aaaa()) + "\r\n";


	


	return result;
}

//1204 譜面インプットまでおｋ　評価クラスと、CNソフラン