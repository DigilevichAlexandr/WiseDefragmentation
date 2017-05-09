#pragma once
#include <string>
#include <windows.h>

#define BITMAP_CHUNK_SIZE 32*1024   // 32 KB is our chunk size to read $bitmap file

namespace WiseDifragmentation {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		array<String^> ^drives;

		void Search(char let);
		int GetClusters(const char* name, bool d);

		DWORD ClusterSize;
		list<string> frag_files;

		void ProcessVolume(char VolumeName);
		int FindFreeBlock(
			HANDLE VolumeHandle,
			ULONG64 MinimumLcn,          /* Cluster must be at or above this LCN. */
			DWORD MinimumSize,           /* Cluster must be at least this big. */
			ULONG64 *BeginLcn,           /* Result, LCN of begin of cluster. */
			ULONG64 *EndLcn);


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 12);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(131, 28);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Информация о диске";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(12, 46);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(131, 21);
			this->comboBox1->TabIndex = 1;
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(12, 73);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(404, 160);
			this->listBox1->TabIndex = 2;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(150, 12);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(128, 28);
			this->button2->TabIndex = 3;
			this->button2->Text = L"Дефрагментировать";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(284, 12);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(132, 28);
			this->button3->TabIndex = 4;
			this->button3->Text = L"Информация о файле";
			this->button3->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(428, 245);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->listBox1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

	}

	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
		drives = System::IO::Directory::GetLogicalDrives();

		for (int i = 0; i < drives->Length; i++)
		{
			this->comboBox1->Items->Add(drives[i]);
		}

		this->comboBox1->SelectedIndex = 0;


	}

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		std::wstring st = L"G:\\";
		//int vol = ReadVolumeBitmap(st);
		//listBox1->Items->Add(String::Format("result {0}", vol));


	}

	private:int ReadVolumeBitmap(std::wstring volumeName)
	{
		HANDLE hVolume;
		STARTING_LCN_INPUT_BUFFER startingLcn;
		VOLUME_BITMAP_BUFFER *volBitmap;
		UINT32 bitmapSize;
		DWORD bytesReturned, bytesWritten;
		BOOL ret, retFile;
		HANDLE hFile;
		std::wstring bitmapFile;
		int result;

		/* Open the volume for reading */
		hVolume = CreateFile(
			volumeName.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		startingLcn.StartingLcn.QuadPart = 0;
		bitmapSize = BITMAP_CHUNK_SIZE + sizeof(LARGE_INTEGER) * 2;
		volBitmap = (VOLUME_BITMAP_BUFFER *)malloc(bitmapSize);

		bitmapFile = L"1.txt";

		hFile = CreateFile(
			bitmapFile.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			0,
			NULL);


		/* Read $bitmap file in loop */
		while (TRUE) {
			ret = DeviceIoControl(
				hVolume,
				FSCTL_GET_VOLUME_BITMAP,
				&startingLcn,
				sizeof(STARTING_LCN_INPUT_BUFFER),
				volBitmap,
				bitmapSize,
				&bytesReturned,
				NULL);

			if (startingLcn.StartingLcn.QuadPart != volBitmap->StartingLcn.QuadPart) {
				/* some ioctl error */
				return -1;
			}
			if (FALSE == ret) {
				/* only error we expect is to read more data */
				if (GetLastError() != ERROR_MORE_DATA) {
					/* some ioctl error */
					return -1;
				}
			}

			/* find out the exact bytes read from $bitmap */
			bytesReturned -= sizeof(LARGE_INTEGER) * 2;
			retFile = WriteFile(
				hFile,
				volBitmap->Buffer,
				bytesReturned,
				&bytesWritten,
				NULL);
			/* If eof occured for reading $bitmap break the loop */
			if (TRUE == ret) {
				break;
			}
			/* Update the read offset for next request */
			startingLcn.StartingLcn.QuadPart += bytesReturned * 8;
		}

		CloseHandle(hFile);
		CloseHandle(hVolume);
		free(volBitmap);

		return 0;
	}
	};


}
