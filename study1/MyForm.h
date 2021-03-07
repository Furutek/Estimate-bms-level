#pragma once
#include<msclr\marshal_cppstd.h>

namespace study1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm の概要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected:
	private: System::Windows::Forms::Button^  button2;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Button^  button3;


	protected:

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button1->Location = System::Drawing::Point(332, 10);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"open";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button2->Location = System::Drawing::Point(332, 68);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"detect";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(46, 12);
			this->label1->TabIndex = 3;
			this->label1->Text = L"bms file";
			// 
			// textBox1
			// 
			this->textBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox1->Location = System::Drawing::Point(64, 12);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(262, 19);
			this->textBox1->TabIndex = 4;
			// 
			// textBox2
			// 
			this->textBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox2->Location = System::Drawing::Point(14, 97);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox2->Size = System::Drawing::Size(393, 130);
			this->textBox2->TabIndex = 5;
			this->textBox2->Text = L"使い方\r\n1.openボタンでbmsファイルを開く。またはドラッグ＆ドロップも可。\r\n2.detectボタンで解析開始。このボックスに結果を示し、最終行に[完了]"
				L"が表示されたら終わり。";
			// 
			// checkBox1
			// 
			this->checkBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(262, 72);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(64, 16);
			this->checkBox1->TabIndex = 6;
			this->checkBox1->Text = L"do save";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::checkBox1_CheckedChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 44);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(43, 12);
			this->label2->TabIndex = 7;
			this->label2->Text = L"save to";
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(64, 41);
			this->textBox3->Name = L"textBox3";
			this->textBox3->ReadOnly = true;
			this->textBox3->Size = System::Drawing::Size(262, 19);
			this->textBox3->TabIndex = 8;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(332, 39);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 9;
			this->button3->Text = L"change";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// MyForm
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(419, 239);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &MyForm::MyForm_DragDrop);
			this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &MyForm::MyForm_DragEnter);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		OpenFileDialog^ dlg = gcnew OpenFileDialog();
		dlg->Filter = "BMS形式(*.bms)|*.bms|すべてのファイル(*.*)|*.*";
		if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK){
			return;
		}
		System::Diagnostics::Debug::WriteLine(dlg->FileName);
		textBox1->Text = dlg->FileName;
	}



	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		textBox2->Clear();
		textBox2->Text = textBox1->Text;
		textBox2->AppendText(Environment::NewLine + Environment::NewLine);

		msclr::interop::marshal_context context;

		textBox2->AppendText(msclr::interop::marshal_as<System::String^>(execute(context.marshal_as<const char*>(textBox1->Text))));
//		if (checkBox1->Checked){
//			System::IO::StreamWriter^ Writer = gcnew System::IO::StreamWriter(textBox3->Text, false, System::Text::Encoding::GetEncoding("shift-jis"));

//		}
	}

	private: std::string execute(const char*);



private: System::Void MyForm_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
	if (e->Data->GetDataPresent(DataFormats::FileDrop)){
		e->Effect = DragDropEffects::All;
	}
}



private: System::Void MyForm_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
	array<String^>^ files = dynamic_cast<array<String^>^>(e->Data-> GetData(DataFormats::FileDrop));
	textBox1->Text = files[0];
	msclr::interop::marshal_context context;
	for (int i = 0; i < files->Length; i++)
		textBox2->AppendText(msclr::interop::marshal_as<System::String^>(execute(context.marshal_as<const char*>(files[i]))));
	//236以下を付け加えた
}
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
	SaveFileDialog^ dlg = gcnew  SaveFileDialog();
	dlg->Filter = "txt形式(*.txt)|*.txt|すべてのファイル(*.*)|*.*";
	if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK){
		return;
	}
	textBox3->Text = dlg->FileName;
}
};
}
