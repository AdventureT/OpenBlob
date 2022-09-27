#include "pch.h"
#include "AExampleClass.h"
#include "ARootTask.h"

#include <Toshi/File/TFile.h>
#include <Toshi/Core/TSystem.h>
#include <Toshi/Core/TKernelInterface.h>
#include <Toshi/Core/TScheduler.h>
#include <Toshi/Math/TRandom.h>
#include <Toshi/File/TTRB.h>
#include <Toshi/Core/TGenericDList.h>
#include "MemoryCard/EnSaveData.h"
#include <Toshi/Core/TMemory.cpp>

AExampleClass::AExampleClass()
{
	TOSHI_INFO("Called AExampleClass");

	Toshi::TRandom rnd;
	TOSHI_INFO("=================TRandom Test=================");
	TOSHI_INFO("1) Random int (9000, 10000): {0}", rnd.GetInt(9000, 10000));
	TOSHI_INFO("2) Random int (9000, 10000): {0}", rnd.GetInt(9000, 10000));
	TOSHI_INFO("3) Random int (9000, 10000): {0}", rnd.GetInt(9000, 10000));
	TOSHI_INFO("4) Random int (9000, 10000): {0}", rnd.GetInt(9000, 10000));
	TOSHI_INFO("5) Random float: {0}", rnd.GetFloat());
	TOSHI_INFO("6) Random float: {0}", rnd.GetFloat());
	TOSHI_INFO("7) Random float: {0}", rnd.GetFloat());
	TOSHI_INFO("8) Random float: {0}", rnd.GetFloat());
	TOSHI_INFO("9) Random int (500): {0}", rnd.GetInt(500));
	TOSHI_INFO("10) Random int (500): {0}", rnd.GetInt(500));
	TOSHI_INFO("11) Random int (500): {0}", rnd.GetInt(500));
	TOSHI_INFO("12) Random int (500): {0}", rnd.GetInt(500));
	TOSHI_INFO("13) Random int: {0}", rnd.GetInt());
	TOSHI_INFO("14) Random int: {0}", rnd.GetInt());
	TOSHI_INFO("15) Random int: {0}", rnd.GetInt());
	TOSHI_INFO("16) Random int: {0}", rnd.GetInt());
	TOSHI_INFO("==============================================");

	auto fs = Toshi::TFileSystem::CreateNative("local");
	auto file = fs->CreateFile("C:\\Program Files (x86)\\Steam\\steamapps\\common\\de Blob\\Data\\BlobChar\\AssetPack.trb", Toshi::TFile::OpenFlags_Read);
	
	if (file)
	{
		char sos[0x800]{};
		file->Seek(5, Toshi::TFile::TSEEK_SET);
		int read = file->Read(&sos, sizeof(sos));
		
		fs->SetPrefix("C:/");

		TOSHI_INFO("File size: {0} bytes", file->GetSize());
		file->Seek(5, Toshi::TFile::TSEEK_SET);
		TOSHI_INFO("Seeked to offset: {0}", file->Tell());
		fs->DestroyFile(file);
	}

	fs = Toshi::TFileSystem::CreateNative("local");
	file = fs->CreateFile("C:\\Program Files (x86)\\Steam\\userdata\\180297931\\532320\\remote\\SaveData.dat.old", Toshi::TFile::OpenFlags_Read);

	unsigned char* buf = (unsigned char*)Toshi::tmalloc(file->GetSize());//-12);
	//file->Seek(12, Toshi::TFile::TSEEK_CUR);
	file->Read(buf, file->GetSize());//-12);
	
	// This is only for testing purposes
	*(int*)(&buf[8]) = 0; // Setting the CRC to 0 so the new CRC doesn't take the old one and CRCs it

	EnSaveData::GenerateCRC();
	uint32_t crc = EnSaveData::CalculateCRC(buf, file->GetSize());

	Toshi::TTRB trb;
	
	//trb.LoadTrb("D:\\Barnyard\\Game\\Data\\Terrain\\EnvMain\\Beady_L0Mod0.trb");
	//trb.LoadTrb("F:\\Nicktoons Unite\\DATA\\MATLIBS\\BKG_LOAD_DP_NTSC.TTL");
	//trb.GetSect("TTL");

	trb.LoadTrb("C:\\Program Files (x86)\\Steam\\steamapps\\common\\de Blob\\Data\\BlobChar\\AssetPack.trb");
	trb.GetSymb("SkeletonHeader");
	//trb.LoadTrb("C:\\Users\\nepel\\Desktop\\BKG_CONCEPT01_NTSC_ENG.TTL");

	

	class Test : public Toshi::TDList<Test>::TNode
	{
	public:
		Test(float value) : m_Value(value) { }

		inline Test* Next() { return TNode::Next()->As<Test>(); }
		inline float GetValue() const { return m_Value; }

		inline void Log() const { TOSHI_INFO(m_Value); }
	private:
		float m_Value;
	};

	Toshi::TDList<Test> list;
	list.InsertHead(Toshi::tnew<Test>(1.0f));
	list.InsertHead(Toshi::tnew<Test>(5.0f));
	
	list.GetFirst()->Log();         // 5.0
	list.GetFirst()->Next()->Log(); // 1.0

	Toshi::tdelete(list.GetFirst()->Next());
	Toshi::tdelete(list.GetFirst());

	auto kernel = Toshi::tnew<Toshi::TKernelInterface>();
	
	m_RootTask = static_cast<ARootTask*>(
		kernel->GetScheduler().CreateTask(&ARootTask::s_Class, nullptr)
	);

	m_RootTask->SetKernelInterface(kernel);
	m_RootTask->Create();

	TOSHI_INFO("Started ARootTask for 5 seconds...");

	while (m_RootTask->IsActive())
	{
		kernel->Update();
	}

	kernel->Delete();
}

AExampleClass::~AExampleClass()
{
	TOSHI_INFO("Called ~AExampleClass");
}