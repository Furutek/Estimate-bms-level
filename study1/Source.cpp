#include"MyForm.h"

using namespace study1;

[STAThreadAttribute]
int main(){
	MyForm ^fm = gcnew MyForm();
	fm->ShowDialog();
	return 0;
}