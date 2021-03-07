//execute�̒�`�t�@�C���B���ʂ̃p�X��������āA�t�@�C���I�[�v�����ăN���Xsong�ɕ��ʂ�n���A�N���Xsong���番�͌��ʂ�������ďo�͕��͂����֐��B

#include "MyForm.h"
#include"bar.h"
#include"song.h"

#include<string>
#include<iostream>
#include<fstream>
#include<cctype>
#include<algorithm>

using namespace std;

//�O���[�o���ϐ��A�}�W�b�N�i���o�[�͂���
const int resolution = 384;//���ʏc�𑜓x
const double critical = 2.0;//�͂��ꌟ��Aoutput_def_list
const double effect = 1.0;//difeff
const double zonewidth = 0.5;//difficult���U�K�w���̕�

string study1::MyForm::execute(const char* filename){

	string result;
	string buffer;
	string str;
	string::size_type i;
	vector<bar>::size_type j;
	int k;

	//bms�t�@�C�����I�[�v��
	ifstream fis(filename);
	if (!fis.is_open()) {
		result = "�t�@�C�����J���܂���ł����B";
		return result;
	}

	//�Ȋi�[�N���X�����
	song songs;
	result = "����\r\n\r\n";

	//bms�t�@�C�������P
	//�܂���song��bpm�Ə��ߐ�measure���擾�B�擾�ł��邩�ǂ�����bms�t�@�C���̗v���Ƃ���B
	while (!fis.eof()){
		getline(fis, buffer);

		//bms�t�@�C������bpm���擾����song.bpm�ɓ����B�i�s���j#BPM�i�C�Ӑ��X�y�[�X�j***�̌`
		if (buffer.find("#BPM ") == 0){
			i = 0;
//			while ( buffer.at(i) < '0' || '9' < buffer.at(i))i++;
			songs.set_bpm(stoi(buffer.substr(5)));
		}
		//bms�t�@�C�����珬�ߐ����擾����song.measure�ɓ����B�i�s���j#aaabb�̌`��aaa�̍ő�l���擾
		if (buffer.size() >= 9 && buffer.at(0) == '#'    && std::all_of(buffer.begin() + 1, buffer.begin() + 4, isdigit)){
			if (songs.get_measure() < stoi(buffer.substr(1, 3))) songs.set_measure(stoi(buffer.substr(1, 3)));
			//���ߓ���BPM�ω�������\�t��������������t���O�𗧂Ă�B������Ȃ������炻�����̃t���O�𗧂Ă�B
			if (buffer.at(4) == '0' && (buffer.at(5) == '3' || buffer.at(5) == '8')){
				if (buffer.size() == 9){
					songs.soflan_head_on();
				}
				else{
					songs.soflan_mid_on();
				}
			}
		}
		//�V���O���v���C�����m�F
		if (buffer.find("#PLAYER") == 0){
			if (buffer.substr(8) != "1") {
				result = "����V���O���v���C���������Ă܂���\r\n";
				return result;
			}
		}
		//�^�C�g�����擾
		if (buffer.find("#TITLE") == 0){
			if (buffer.size() >= 8)	result += "title:\t" + buffer.substr(7) + "\r\n";
			else result += "title:\t[none]\r\n";
		}
		//LNOBJ���擾
		if (buffer.find("#LNOBJ") == 0){
			if (buffer.size() >= 9) songs.set_lnobj(buffer.substr(7));
		}
	}
	//bms�v���̌����Bbpm��measure��0�łȂ��B
	if (songs.get_bpm() == 0){
		result = "bpm���擾�ł��܂���ł����B���҂����`�́A#BPM ***�ł��B\r\n";
		return result;
	}
	if (songs.get_measure() == 0){
		result = "���ߐ����擾�ł��܂���ł����B���҂����`�́A#aaabb�ł��B\r\n";
		return result;
	}
	//���ߐ�measure�́A000~max(aaa)��aaa+1�B
	else songs.set_measure(songs.get_measure() + 1);


	//songs.bars���������imeasure�ˑ��jbar�̎�ނ����肵�Ă���B
	songs.set_bars_init();

	//bms�t�@�C�������Q
	//�e���߂̔��q��songs.bars[].beat�Ɏ擾�B���ł�#difficulty��
	fis.clear();
	fis.seekg(0, ios_base::beg);
	while (!fis.eof()){
		getline(fis, buffer);
		if (buffer.size() >= 9 && buffer.at(0) == '#'    && std::all_of(buffer.begin() + 1, buffer.begin() + 4, isdigit) && buffer.at(4) == '0' && buffer.at(5) == '2'){
			songs.bars.at(static_cast<std::vector<bar>::size_type>(stoi(buffer.substr(1, 3))))->set_beat(stof(buffer.substr(7)));
		}
		//		if (buffer.find("DIFFICULTY")){} �߂�ǂ������Ȃ�����
		//#bpmbb xxx�̑Ή��\�����B
		if (buffer.find("#BPM") == 0 && buffer.at(4) != ' ') songs.set_bpm_table(stoi(buffer.substr(4, 2), nullptr, 16), stoi(buffer.substr(7)));
	}

	//songs.bars[].fumen���������ibeat�ˑ��j
	songs.set_fumen_init();


	//bms�t�@�C�������R
	//���ʂ���荞�݁B���ۂ̓���̓N���X�ɔC���A�����ł̓t�@�C�������s�擾���ăN���X�ɓn�������B
	fis.clear();
	fis.seekg(0, ios_base::beg);
	while (!fis.eof()){
		getline(fis, buffer);
		songs.input_fumen(buffer);
	}
	fis.clear();
	//bpm�̕ϊ�
	songs.convert_bpm();
	//���߂܂������K�v�ȁA�\�t������LN�̏���
	songs.complement_joint();

	//�S��͒l���N���X�Ɍv�Z�����A�N���X�̃����o�ϐ��Ɋi�[
	songs.read_all();

	//�\��
	result += "bpm:\t" + to_string(songs.get_bpm()) + "\r\n";
	result += "���ߐ�:\t" + to_string(songs.get_measure()) + "\r\n";
	result += "����:\t" + to_string(songs.get_time()) + "[�b]\r\n";
	result += "��������TOTAL�l:\t" + to_string(static_cast<int>((7.605 * songs.get_notes(8)) / (0.01 * songs.get_notes(8) + 6.5))) + "\r\n";
	result += "\r\n";
	result += "���m�[�c��:\t" + to_string(songs.get_notes(8)) + " (LN�͎n�_�̂ݐ�����)\r\n";
	result += "���ϖ��x\t" + to_string(songs.get_density()) + "[notes/�b]\r\n";
	for (int key = 1; key < 8; key++){
		result += to_string(key) + "��:\t" + to_string(songs.get_notes(key)) + "\t";
		if (static_cast<double>(songs.get_notes(key)) / songs.get_notes(8) < 0.1)result += "0";
		result += to_string(static_cast<double>(songs.get_notes(key)) / songs.get_notes(8) * 100) + "%\r\n";
	}
	result += "��M:\t" + to_string(songs.get_notes(0)) + "\t";
	if (static_cast<double>(songs.get_notes(0)) / songs.get_notes(8) < 0.1)result += "0";
	result += to_string(static_cast<double>(songs.get_notes(0)) / songs.get_notes(8) * 100) + "%\r\n";
	result += "\r\n";
	result += "����\t���ߓ�x\t�m�[�c��\t�O���t�i10���݁j\r\n";
	for (j = 0; j < static_cast<std::vector<bar>::size_type>(songs.get_measure()); j++){
		result += to_string(j) + "\t" + to_string(songs.bars.at(j)->get_difficulty()) + "\t" + to_string(songs.bars.at(j)->get_notes(8)) + "\t";
		for (k = 0; k < songs.bars.at(j)->get_notes(8); k++){
			result += "|";
			if (k % 10 == 9) result += " ";
		}
		result += "\r\n";//����N���X���ɒ�`�����ق��������H
	}//�N���X�̓�����draw_notes_graph������ׂ���������Ȃ��B
	result += "\r\n";
	result += songs.write_difzone_list();
	result += "\r\n";
	result += "��Փx����\t" + to_string(songs.get_difave()) + "\r\n";
	result += "��Փx�W���΍�\t" + to_string(songs.get_difsdv()) + "\r\n";
	result += "�L����Փx:\t" + to_string(songs.get_difeff()) + "\r\n";
	//�����s����
	result += "���C����Փx��\t" + to_string(zonewidth * (songs.get_difmain() - 0)) + "~" + to_string(zonewidth * (songs.get_difmain() + 1)) + "\r\n";


	//�f�o�b�O�p
//	for (int k = 0; k < 4; k++) result += to_string(songs.bars[k].aaaaa()) + "\r\n";

//	for (std::vector<bool>::size_type k = 0; k < 384; k++)result += to_string(songs.bars[9].aaaa(k));

//	result += to_string(songs.bars[1].aaaa()) + "\r\n";


	


	return result;
}

//1204 ���ʃC���v�b�g�܂ł����@�]���N���X�ƁACN�\�t����