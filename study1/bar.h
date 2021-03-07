#ifndef ___Class_Bar
#define ___Class_Bar

#include<iostream>
#include<vector>
#include<string>

class song;

extern const int resolution;

class bar
{
	friend song;//開発中とつぜんフレンドにしたから、余計なセッタやゲッタがたくさんある。

protected:

	std::vector<int> bpm;
	std::vector<int> bpm_tick;

	double beat = 1.0;

	//fumen[key].at(tick) = ノーツ有無
	std::vector<bool> fumen[8];
	//cnの仕様は、(swi, sus)が、(0,0)==何もなし,(1,1)==on,(1,0)==おしっぱ,(0,1)==off
	std::vector<bool> cn_swi[8];
	std::vector<bool> cn_sus[8];


	//譜面から読み出すデータ
	int notes[9];//notes[0~7]は鍵盤毎の、notes[8]は小節のノーツ数
	double time;
	double density;
	double difficulty;

public:

	//コンストラクタ
	bar(){
		beat = 1.0;
		notes[0] = 0;
		notes[1] = 0;
		notes[2] = 0;
		notes[3] = 0;
		notes[4] = 0;
		notes[5] = 0;
		notes[6] = 0;
		notes[7] = 0;
		notes[8] = 0;
	}

	//ゲッタセッタ
	void set_bpm(int b, int t) {//そんなに必要ある関数じゃない
		bpm.push_back(b);
		bpm_tick.push_back(t);
	}
	int bpm_ttob(int t){
		for (std::vector<int>::size_type i = 0; i < bpm.size(); i++){
			if (bpm_tick.at(i) == t)
				return bpm.at(i);
		}
		return 0;
	}
	int bpm_last(){
		int tick = 0;
		for (std::vector<int>::size_type i = 0; i < bpm.size(); i++){
			if (bpm_tick.at(i) > tick) tick = bpm_tick.at(i);
		}
		return bpm_ttob(tick);
	}

	void set_beat(double d) { beat = d; }
	double get_beat() { return  beat; }//使うの？


	//
	//読み込み関数
	//
	//この小節のfumenを初期化（beat）依存
	void set_fumen_init();

	//譜面取り込み用関数
	virtual void input_fumen(std::string, std::string);


	//
	//解析関数
	//
	void read_all(){
		read_notes();
		read_time();
		read_density();
		read_difficulty();
	}
	//キー別ノーツ数と、全体ノーツ数
	void read_notes();
	int get_notes(int i){ return notes[i]; }

	//timeはこの小節での時間[sec]
	void read_time();
	double get_time(){ return time; }
	//densityはこの小節での[notes/sec]に決めた。分あたりの方がいい？
	void read_density(){ density = 1.0 * notes[8] / time; }
	double get_density(){ return density; }

	//densityに補正を加える形でdifficultyを計算。
	void read_difficulty();
	double get_difficulty(){ return difficulty; }



	//デバッグ用関数
	std::string aaaa(){
		std::vector<bool>::size_type i = 0;
		std::string str;
		for (; i < fumen[0].size(); i++){
			str += to_string(fumen[4].at(i));
		}
		return str;
	}
	double aaaaa() { return beat; }
};






/*最初はソフランとLNは派生クラスをつくる予定だったけどやめた。

class bar_sof1 : public bar{

	public:
	void input_fumen(std::string);

};


class bar_sof2 : public bar{
	std::vector<int> bpm_tick;
	std::vector<int> bpm_value;
public:
	void input_fumen(std::string);
	void read_time();
	void read_difficulty();
};
*/



#endif