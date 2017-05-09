#pragma once
#include <string>
#include <windows.h>
using std::string;

#define BITMAP_CHUNK_SIZE 32*1024   // 32 KB is our chunk size to read $bitmap file

namespace WiseDifragmentation {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	//template <class T>
	//class List;

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

		//void Search(char let);
		int GetClusters(const char* name, bool d);

		DWORD ClusterSize;
		List<String^>^ frag_files = gcnew List<String^>();
		//List<string> frag_files;

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
		int vol = ReadVolumeBitmap(st);
		listBox1->Items->Add(String::Format("result {0}", vol));


	}

	private:void Search(char let)
	{

		//t=0;
		//files=0;
		frag_files.clear();
		system("cls");
		char name[4];
		sprintf(name, "%c:\\", let);


		HANDLE hDevice = INVALID_HANDLE_VALUE;
		char namef[31];
		sprintf(namef, "\\\\.\\%c:", let);

		hDevice = CreateFileA(namef,
			0, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		DWORD SectorsPerCluster;
		DWORD BytesPerSector;
		DWORD NumberOfFreeClusters;
		DWORD TotalNumberOfClusters;
		strncat(namef, "\\", 1);
		GetDiskFreeSpaceA(namef,
			&SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters);
		ClusterSize = SectorsPerCluster*BytesPerSector;

	}

	private:int GetClusters(const char* name, bool d)
	{


		MOVE_FILE_DATA MoveParams;

		ULONG  ClCount;
		LARGE_INTEGER FileSize;
		HANDLE  hFile;
		ULONG   OutSize;
		ULONG   Bytes;
		LARGE_INTEGER PrevVCN;
		STARTING_VCN_INPUT_BUFFER  InBuf;
		PRETRIEVAL_POINTERS_BUFFER OutBuf;

		hFile = CreateFile(name, FILE_READ_ATTRIBUTES,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
		char namef[MAX_PATH];
		sprintf(namef, "\\\\.\\%c:", name[0]);
		HANDLE hDisk = CreateFile(namef, GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		MoveParams.FileHandle = hFile;

		if (hFile != INVALID_HANDLE_VALUE)
		{
			Search(name[0]);

			GetFileSizeEx(hFile, &FileSize);
			OutSize = (ULONG)sizeof(RETRIEVAL_POINTERS_BUFFER) + (FileSize.QuadPart / ClusterSize) * sizeof(OutBuf->Extents);
			OutBuf = (PRETRIEVAL_POINTERS_BUFFER)malloc(OutSize);
			InBuf.StartingVcn.QuadPart = 0;
			if (DeviceIoControl(hFile, FSCTL_GET_RETRIEVAL_POINTERS, &InBuf, sizeof(InBuf), OutBuf, OutSize, &Bytes, NULL))
			{
				ClCount = (FileSize.QuadPart + ClusterSize - 1) / ClusterSize;

				ProcessVolume(name[0]);
				ULONG64 BLcn = -1, ELcn = -1;
				FindFreeBlock(hDisk, 0, ClCount, &BLcn, &ELcn);
				MoveParams.ClusterCount = 0;
				MoveParams.StartingLcn.QuadPart = BLcn;
				PrevVCN = OutBuf->StartingVcn;

				if (d == 0)
				{
					frag_files.push_back(name);
					int rez = OutBuf->ExtentCount;
					free(OutBuf);
					CloseHandle(hFile);
					return rez;
				}
				for (ULONG r = 0; r < OutBuf->ExtentCount; r++)
				{
					DWORD br;
					MoveParams.StartingVcn = OutBuf->Extents[r].Lcn;
					MoveParams.ClusterCount = OutBuf->Extents[r].NextVcn.QuadPart - PrevVCN.QuadPart;
					////////////////////////////////////////////
					///FSCTL_MOVE_FILE
					////////////////////////////////////////////
					int z = DeviceIoControl(hDisk, FSCTL_MOVE_FILE, &MoveParams, sizeof(MoveParams), NULL, 0, &br, NULL);
					if (!z)
						printf("error %d\n", GetLastError());
					MoveParams.StartingLcn.QuadPart = MoveParams.StartingLcn.QuadPart + MoveParams.ClusterCount;
					PrevVCN = OutBuf->Extents[r].NextVcn;
				}
			}

			free(OutBuf);
			CloseHandle(hFile);
			CloseHandle(hDisk);
		}
		return 1;
	}

	private:ULONG64 MaxLcn;                 /* Highest possible LCN + 1. */
	private:value struct Exclude {
		ULONG64 Start;
		ULONG64 End;
	};
	private:value struct Data {
		ULONG64 StartingLcn;
		ULONG64 BitmapSize;
		array<BYTE>^ Buffer = gcnew array<BYTE>(8);
	};

	private:array <Exclude>^ Excludes = gcnew array <Exclude>(3);

	private:void    ProcessVolume(char VolumeName) {
		HANDLE VolumeHandle;
		STARTING_LCN_INPUT_BUFFER InBuffer;

		int Result;
		NTFS_VOLUME_DATA_BUFFER NtfsData;
		DWORD w;


		/* Initialize. */
		MaxLcn = 0;
		Excludes[0].Start = 0;
		Excludes[0].End = 0;
		Excludes[1].Start = 0;
		Excludes[1].End = 0;
		Excludes[2].Start = 0;
		Excludes[2].End = 0;

		/* Initialize random number generator. */

		char name[7];
		sprintf(name, "\\\\.\\%c:", VolumeName);
		/* Open the VolumeHandle. If error then leave. */
		VolumeHandle = CreateFile(name, GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (VolumeHandle == INVALID_HANDLE_VALUE) {
			printf("Error while opening volume \"%c\"", VolumeName);
			//return INVALID_HANDLE_VALUE;
		}

		/* If the volume is not mounted then leave. Unmounted volumes can be
		defragmented, but the system administrator probably has unmounted
		the volume because he wants it untouched. */
		if (DeviceIoControl(VolumeHandle, FSCTL_IS_VOLUME_MOUNTED, NULL, 0, NULL, 0, &w, NULL) == 0) {
			printf("Skipping volume '%c' because it is not mounted.", VolumeName);
			CloseHandle(VolumeHandle);
			//return INVALID_HANDLE_VALUE;
		}

		/* Determine the maximum LCN. A single call to FSCTL_GET_VOLUME_BITMAP
		is enough, we don't have to walk through the entire bitmap.
		It's a pity we have to do it in this roundabout manner, because
		there is no system call that reports the total number of clusters
		in a volume. GetDiskFreeSpace() does, but is limited to 2Gb volumes,
		GetDiskFreeSpaceEx() reports in bytes, not clusters, and
		FSCTL_GET_NTFS_VOLUME_DATA only works for NTFS volumes. */
		InBuffer.StartingLcn.QuadPart = MaxLcn;
		Result = DeviceIoControl(VolumeHandle, FSCTL_GET_VOLUME_BITMAP,
			&InBuffer, sizeof(InBuffer),
			&Data, sizeof(Data),
			&w, NULL);
		if (Result == 0) {
			Result = GetLastError();
			if (Result != ERROR_MORE_DATA) {
				printf("Cannot defragment volume: %c", VolumeName);
				CloseHandle(VolumeHandle);
				//return INVALID_HANDLE_VALUE;
			}
		}
		MaxLcn = Data.StartingLcn + Data.BitmapSize;

		/* Setup the list of clusters that cannot be used. The Master File
		Table cannot be moved and cannot be used by files. All this is
		only necessary for NTFS volumes. */
		Result = DeviceIoControl(VolumeHandle, FSCTL_GET_NTFS_VOLUME_DATA,
			NULL, 0, &NtfsData, sizeof(NtfsData), &w, NULL);
		if (Result != 0) {
			/* Note: NtfsData.TotalClusters.QuadPart should be exactly the same
			as the MaxLcn that was determined in the previous block. */
			Excludes[0].Start = NtfsData.MftStartLcn.QuadPart;
			Excludes[0].End = NtfsData.MftStartLcn.QuadPart +
				NtfsData.MftValidDataLength.QuadPart / NtfsData.BytesPerCluster;
			Excludes[1].Start = NtfsData.MftZoneStart.QuadPart;
			Excludes[1].End = NtfsData.MftZoneEnd.QuadPart;
			Excludes[2].Start = NtfsData.Mft2StartLcn.QuadPart;
			Excludes[2].End = NtfsData.Mft2StartLcn.QuadPart +
				NtfsData.MftValidDataLength.QuadPart / NtfsData.BytesPerCluster;

			/* Show debug info. */
		}
		//return VolumeHandle;
		/* Close the volume handle. */
		CloseHandle(VolumeHandle);

		/* Cleanup the FileList. */
	}

	private:int FindFreeBlock(
		HANDLE VolumeHandle,
		ULONG64 MinimumLcn,          /* Cluster must be at or above this LCN. */
		DWORD MinimumSize,           /* Cluster must be at least this big. */
		ULONG64 *BeginLcn,           /* Result, LCN of begin of cluster. */
		ULONG64 *EndLcn) {           /* Result, LCN of end of cluster. */
		STARTING_LCN_INPUT_BUFFER InBuffer;
		value struct {
			ULONG64 StartingLcn;
			ULONG64 BitmapSize;
			//BYTE Buffer[32768];           /* Most efficient if binary multiple. */
			array<BYTE>^ Buffer = gcnew array<BYTE>(32768);
		} Data;

		ULONG64 Lcn;
		ULONG64 ClusterStart;
		int Index;
		int IndexMax;
		BYTE Mask;
		int InUse;
		int PrevInUse;
		int Result;
		DWORD w;

		/* Main loop to walk through the entire clustermap. */
		Lcn = MinimumLcn;
		ClusterStart = 0;
		PrevInUse = 1;
		do {

			/* Sanity check. */
			if ((MaxLcn > 0) && (Lcn >= MaxLcn)) return 0;

			/* Fetch a block of cluster data. */
			InBuffer.StartingLcn.QuadPart = Lcn;
			Result = DeviceIoControl(VolumeHandle, FSCTL_GET_VOLUME_BITMAP,
				&InBuffer, sizeof(InBuffer),
				&Data, sizeof(Data),
				&w, NULL);
			if (Result == 0) {
				Result = GetLastError();
				if (Result != ERROR_MORE_DATA) {
					return 0;
				}
			}

			/* Analyze the clusterdata. We resume where the previous block left
			off. If a cluster is found that matches the criteria then return
			it's LCN (Logical Cluster Number). */
			Lcn = Data.StartingLcn;
			Index = 0;
			Mask = 1;
			IndexMax = sizeof(Data.Buffer);
			if (Data.BitmapSize / 8 < IndexMax) IndexMax = (int)(Data.BitmapSize / 8);
			while (Index < IndexMax) {
				InUse = (Data.Buffer[Index] & Mask);
				if (((Lcn >= Excludes[0].Start) && (Lcn < Excludes[0].End)) ||
					((Lcn >= Excludes[1].Start) && (Lcn < Excludes[1].End)) ||
					((Lcn >= Excludes[2].Start) && (Lcn < Excludes[2].End))) {
					InUse = 1;
				}
				if ((PrevInUse == 0) && (InUse != 0)) {

					if ((ClusterStart >= MinimumLcn) &&
						(Lcn - ClusterStart >= MinimumSize)) {
						*BeginLcn = ClusterStart;
						if (EndLcn != NULL) *EndLcn = Lcn;
						return 1;
					}
				}
				if ((PrevInUse != 0) && (InUse == 0)) ClusterStart = Lcn;
				PrevInUse = InUse;
				if (Mask == 128) {
					Mask = 1;
					Index = Index + 1;
				}
				else {
					Mask = Mask << 1;
				}
				Lcn = Lcn + 1;
			}

		} while ((Result == ERROR_MORE_DATA) &&
			(Lcn < Data.StartingLcn + Data.BitmapSize));

		if (PrevInUse == 0) {
			if ((ClusterStart >= MinimumLcn) &&
				(Lcn - ClusterStart >= MinimumSize)) {
				*BeginLcn = ClusterStart;
				if (EndLcn != NULL) *EndLcn = Lcn;
				return 1;
			}
		}

		return 0;
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
