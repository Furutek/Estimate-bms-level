#ifndef ___Class_Song
#define ___Class_Song

#include"bar.h"
#include<vector>
#include<iostream>
#include<math.h>

extern const double critical;
extern const double effect;
extern const double zonewidth;

class song
{
	//譜面取り込みに必要なデータ
	int bpm;
	int measure;
	
	bool soflan_head;
	bool soflan_mid;
	bool cn_ch5;
	bool cn_lnobj;
	bool stop;
	bool mine;

	//#BPMbb xxの対応表を作る
	std::vector<int> bpm_table1;//bb
	std::vector<int> bpm_table2;//xx

	//#LNOBJ ZZ
	std::string lnobj;

	//譜面から読み出すデータ
	int notes[9];
	double time;
	//density平均（０は抜いてある）
	double density;

	//difficulty平均
	double difave;
	//difficulty標準偏差
	double difsdv;
	//有効difficulty（忘れた）
	double difeff;
	//difficultyをbandwidth刻みで区分けしたときの小節数
	std::vector<int> difzone;
	//difzoneの添え字という形
	std::vector<int>::size_type difmain;

public:

	//クラスbar型を並べたベクトル。譜面を格納する
	std::vector<bar*> bars;

	//コンストラクタ
	song(){
		bpm = 0;
		measure = 0;
		soflan_head = 0;
		soflan_mid = 0;
		cn_ch5 = 0;
		cn_lnobj = 0;
		stop = 0;
		mine = 0;
		lnobj = "ZZZZ";
	}

	//privateデータメンバのセッタとゲッタ
	void set_bpm(int i) { bpm = i; }
	int get_bpm() const { return bpm; }
	void set_measure(int i){ measure = i; }
	int get_measure() const { return measure; }

	void set_bpm_table(int i, int j) { 
		bpm_table1.push_back(i);
		bpm_table2.push_back(j);
	}
	int get_bpm_table(int j) {
		for (std::vector<int>::size_type i = 0; i < bpm_table1.size(); i++){
			if (j == bpm_table1.at(i))return bpm_table2.at(i);
		}
		return 10000;
	}

	void set_lnobj(std::string str){ lnobj = str; }
//	std::string get_lnobj() { return lnobj; }

	void soflan_head_on(){ soflan_head = 1; }
	void soflan_mid_on(){ soflan_mid = 1; }
	void cn_ch5_on(){ cn_ch5 = 1; }
	void cn_lnobj_on(){ cn_lnobj; }

	//
	//読み込み関数
	//
	//拡張bpmの変換
	void convert_bpm();
	//barsを初期化（measure依存）
	void set_bars_init();

	//""すべての""bars.fumenを初期化（bars.beat依存）
	void set_fumen_init();

	//譜面インプット。小節数を読んで渡すだけ。
	void input_fumen(std::string);

	//
	void complement_joint();

	//
	//解析関数
	//
	void read_all(){
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			bars.at(i)->read_all();
		}
		read_notes();
		read_time();
		read_density();
		read_difave();
		read_difsdv();

		read_difzone();
		read_difmain();

		read_difeff();
	}

	//キー別ノーツ数と全体ノーツ数をオーバーロード
	void read_notes();
	int get_notes(int i){ return notes[i]; }

	//曲の時間
	void read_time();
	double get_time(){ return time; }

	//bars.densityの重み付け平均。とりあえず0は省いてる。
	void read_density();
	double get_density(){ return density; }

	//
	//統計計算
	//
	//difficulty平均
	void read_difave();
	double get_difave(){ return difave; }

	//difficulty標準偏差
	void read_difsdv();
	double get_difsdv(){ return difsdv; }
	//difficultyをリストにして記述
	std::string output_dif_list();

	//有効difficulty
	void read_difeff();
	double get_difeff(){ return difeff; }

	//difficultyゾーン分け
	void read_difzone();
	std::string write_difzone_list();

	//メインdifficultyゾーン
	void read_difmain();
	std::vector<int>::size_type get_difmain(){return difmain;}

	
	//ここから下実験中
	void read_difmain3();
	
	double difeff1(){
		double bunshi = 0;
		double time = 0;
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			//ここ不安定
			if (bars.at(i)->get_difficulty() > difave){
				bunshi += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
				time += bars.at(i)->get_time();
			}
		}
		return bunshi / time;
	}

	double difeff2(){
		double bunshi = 0;
		double time = 0;
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			//ここ不安定
			if (bars.at(i)->get_difficulty() > (difave-difsdv)){
				bunshi += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
				time += bars.at(i)->get_time();
			}
		}
		return bunshi / time;
	}

	double difeff3(){
		double bunshi = 0;
		double time = 0;
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			//ここ不安定
			if (bars.at(i)->get_difficulty() > zonewidth * difmain){
				bunshi += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
				time += bars.at(i)->get_time();
			}
		}
		return bunshi / time;
	}

	double difeff4(){
		double bunshi = 0;
		double time = 0;
		std::vector<int>::size_type counter = 0;
		std::vector<int>::size_type j = difzone.size() - 1;
		for (; j > 0; j--){
			counter += difzone.at(j);
			if (counter >= static_cast<std::vector<int>::size_type>(measure / 2))
				break;
		}
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			//ここ不安定
			if (bars.at(i)->get_difficulty() > zonewidth * j){
				bunshi += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
				time += bars.at(i)->get_time();
			}
		}
		return bunshi / time;
	}


	//デバッグ用関数
//	std::vector<bool>::size_type aaa(std::vector<bar>::size_type i){return bars[i].aaaa(); }

};







#endif