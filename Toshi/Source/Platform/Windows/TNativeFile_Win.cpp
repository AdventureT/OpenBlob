#include "ToshiPCH.h"
#include "TNativeFile_Win.h"

#define BUFFER_SIZE 0x800

namespace Toshi
{
    bool TFileManager::Create()
    {
        CreateCommon();
        auto fileManager = TFileManager::GetSingleton();

        CHAR currentDir[0x200];
        DWORD dirLength = GetCurrentDirectoryA(sizeof(currentDir), currentDir);
        TASSERT(dirLength > 0, "The directory's length is 0");

        TString8 prefix(currentDir);

        TFileSystem* pFileSystem;
        pFileSystem = new TNativeFileSystem("local");
        pFileSystem->SetPrefix(prefix);
        pFileSystem = new TNativeFileSystem("abs");
        pFileSystem->SetPrefix(prefix);
        pFileSystem = new TNativeFileSystem("null");
        pFileSystem->SetPrefix(prefix);

        fileManager->SetSystemPath("local");

        return true;
    }

#pragma region TNativeFileSystem

    TNativeFileSystem::TNativeFileSystem(const char* name) : TFileSystem(name)
    {
        m_Handle = INVALID_HANDLE_VALUE;
        TFileManager::GetSingleton()->MountFileSystem(this);
    }

    TFile* TNativeFileSystem::CreateFile(TString8 const& fn, uint32_t flags)
    {
        TNativeFile* nativeFile = new TNativeFile(this);
        //new (nativeFile) TNativeFile(this);

        if (!nativeFile->Open(fn, flags))
        {
            delete nativeFile;
            return TNULL;
        }

        return nativeFile;
    }

    void TNativeFileSystem::DestroyFile(TFile* file)
    {
        if (file != TNULL)
        {
            static_cast<TNativeFile*>(file)->Close();
        }
    }

    bool TNativeFileSystem::MakeDirectory(TString8 const& string)
    {
        return CreateDirectoryA(string, TNULL);
    }

    bool TNativeFileSystem::GetNextFile(TString8& fileName, uint32_t flags)
    {
        WIN32_FIND_DATAA findFileData;

        if (m_Handle != INVALID_HANDLE_VALUE)
        {
            bool bResult = FindNextFileA(m_Handle, &findFileData);
            if (!bResult)
            {
                m_Handle = INVALID_HANDLE_VALUE;
            }
            else if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if ((flags & 1) != 0)
                {
                    fileName = findFileData.cFileName;
                    return true;
                }
            }
            else if ((flags & 2) != 0)
            {
                fileName = findFileData.cFileName;
                return true;
            }
        }

        return false;
    }

#pragma endregion

    bool TNativeFile::LoadBuffer(DWORD bufferPos)
    {
        // FUN_00689ff0
        DWORD lpNumberOfBytesRead;
        TASSERT(m_RBuffer != TNULL, "m_RBuffer doesn't exist");

        if (m_PrevBufferPos != bufferPos)
        {
            if (m_RBufferPosition != bufferPos)
            {
                m_RBufferPosition = SetFilePointer(m_Handle, bufferPos, TNULL, FILE_BEGIN);
                if (m_RBufferPosition != bufferPos) return false;
            }

            if (ReadFile(m_Handle, m_RBuffer, BUFFER_SIZE, &lpNumberOfBytesRead, TNULL) == 0)
            {
                return false;
            }

            m_RBufferPosition += lpNumberOfBytesRead;
            m_LastBufferSize = lpNumberOfBytesRead;
            m_PrevBufferPos = bufferPos;
        }

        return true;
    }

    int TNativeFile::FlushWriteBuffer()
    {
        DWORD lpNumberOfBytesWritten;

        if (m_Position != m_RBufferPosition)
        {
            TASSERT(TFALSE == m_UseBuffers || m_WBufferIterator == 0, "");
            m_RBufferPosition = SetFilePointer(m_Handle, m_Position, TNULL, FILE_BEGIN);
            
            if (m_RBufferPosition == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
            {
                return 0;
            }

            m_Position = m_RBufferPosition;
        }

        if (WriteFile(m_Handle, m_WBuffer, m_WBufferIterator, &lpNumberOfBytesWritten, TNULL) == 0)
        {
            return 0;
        }

        m_RBufferPosition += lpNumberOfBytesWritten;
        m_Position = m_RBufferPosition;
        m_WBufferIterator = 0;
        return lpNumberOfBytesWritten;
    }

    int TNativeFile::ReadUnbuffered(LPVOID dst, size_t size)
    {
        DWORD lpNumberOfBytesRead;
        FlushWriteBuffer();

        if (m_Position != m_RBufferPosition)
        {
            m_RBufferPosition = SetFilePointer(m_Handle, m_Position, TNULL, FILE_BEGIN);
            if (m_RBufferPosition == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
            {
                return 0;
            }
            m_Position = m_RBufferPosition;
        }

        if (ReadFile(m_Handle, dst, size, &lpNumberOfBytesRead, TNULL) == 0)
        {
            return 0;
        }

        m_RBufferPosition += lpNumberOfBytesRead;
        m_Position = m_RBufferPosition;

        return lpNumberOfBytesRead;
    }

    size_t TNativeFile::Read(LPVOID dst, size_t size)
    {
        FlushWriteBuffer();

        if (size < 1) { return 0; }

        if (m_RBuffer != TNULL)
        {
            DWORD readedCount = 0;
            DWORD startPos = m_Position;
            DWORD curBufferPos = startPos / BUFFER_SIZE * BUFFER_SIZE;
            DWORD newBufferPos = (startPos + size) / BUFFER_SIZE * BUFFER_SIZE;
            LPVOID curPosBuffer = dst;

            if (curBufferPos != newBufferPos)
            {
                if (curBufferPos == m_PrevBufferPos)
                {
                    DWORD readCount = m_LastBufferSize - (startPos - curBufferPos);

                    if (readCount > 0)
                    {
                        memcpy(dst, m_RBuffer + startPos - curBufferPos, readCount);
                        
                        curPosBuffer = (char*)dst + readCount;
                        m_Position += readCount;
                        readedCount = readCount;
                    }
                }

                DWORD toReadCount = newBufferPos - m_Position;
                curBufferPos = newBufferPos;

                if (toReadCount > 0)
                {
                    uint32_t readed = ReadUnbuffered(curPosBuffer, toReadCount);
                    curPosBuffer = (char*)curPosBuffer + readed;
                    readedCount += readed;

                    if (readed != toReadCount)
                    {
                        // end of file?
                        return readedCount;
                    }
                }
            }

            if (readedCount != size && LoadBuffer(curBufferPos))
            {
                size -= readedCount;
                DWORD bufferLeftSize = m_Position - curBufferPos;
                DWORD readCount = m_LastBufferSize - bufferLeftSize;
                readCount = TMath::Min(readCount, size);

                if (readCount > 0)
                {
                    memcpy(curPosBuffer, m_RBuffer + bufferLeftSize, readCount);
                    m_Position += readCount;
                    readedCount += readCount;
                }
            }

            return readedCount;
        }

        return ReadUnbuffered(dst, size);;
    }

    uint32_t TNativeFile::Tell()
    {
        FlushWriteBuffer();
        return m_Position;
    }


    bool TNativeFile::Seek(int offset, TFile::TSEEK origin)
    {
        FlushWriteBuffer();

        if (origin == TFile::TSEEK_SET)
        {
            m_Position = offset;
        }
        else if (origin == TFile::TSEEK_CUR)
        {
            m_Position += offset;
        }
        else if (origin == TFile::TSEEK_END)
        {
            m_RBufferPosition = SetFilePointer(m_Handle, offset, TNULL, FILE_END);
            
            if (m_RBufferPosition == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
            {
                return false;
            }

            m_Position = m_RBufferPosition;
        }

        if (m_Position < 0) m_Position = 0;

        return true;
    }

    char TNativeFile::GetCChar()
    {
        FlushWriteBuffer();

        if (m_RBuffer != TNULL)
        {
            uint32_t curBufferPos = m_Position / BUFFER_SIZE * BUFFER_SIZE;
            if ((curBufferPos == m_PrevBufferPos) && (m_Position - curBufferPos <= m_LastBufferSize - 1))
            {
                char c = m_RBuffer[m_Position - curBufferPos];
                m_Position++;
                return c;
            }
        }
        //   iVar3 = (*(code *)*this->field_0x0)(&stack0xfffffff4,1);
        //   if (iVar3 != 1) {
        //      return 0xffffffff;
        //   }
        //   return puVar4 & 0xFF;
        return 0;
    }

    int TNativeFile::GetSize()
    {
        m_RBufferPosition = SetFilePointer(m_Handle, 0, TNULL, TSEEK_END);
        
        if (m_RBufferPosition == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
        {
            return 0;
        }

        return m_RBufferPosition;
    }

    _FILETIME TNativeFile::GetDate()
    {
        _FILETIME fLastWriteTime;
        fLastWriteTime.dwLowDateTime = 0;
        fLastWriteTime.dwHighDateTime = 0;

        if (m_Handle != TNULL)
        {
            GetFileTime(m_Handle, NULL, NULL, &fLastWriteTime);
        }
        
        return fLastWriteTime;
    }

    TNativeFile::TNativeFile(TNativeFileSystem* pFS) : TFile(pFS)
    {
        m_Handle = INVALID_HANDLE_VALUE;
        m_Position = -1;
        m_RBufferPosition = -1;
        m_PrevBufferPos = -1;
        m_LastBufferSize = 0;
        m_RBuffer = TNULL;
        m_WBuffer = TNULL;
        m_WBufferIterator = 0;
        m_UseBuffers = true;
    }

    TNativeFile::TNativeFile(const TNativeFile& other) : TFile(other)
    {
        m_Handle = other.m_Handle;
        m_Position = other.m_Position;
        m_RBufferPosition = other.m_RBufferPosition;
        m_PrevBufferPos = other.m_PrevBufferPos;
        m_LastBufferSize = other.m_LastBufferSize;
        m_RBuffer = other.m_RBuffer;
        m_WBuffer = other.m_WBuffer;
        m_WBufferIterator = other.m_WBufferIterator;
        m_UseBuffers = other.m_UseBuffers;
    }

    bool TNativeFile::Open(const TString8& a_FileName, uint32_t a_Flags)
    {
        TASSERT(a_FileName.IsIndexValid(0), "TNativeFile::Open - wrong filename");

        DWORD dwCreationDisposition = OPEN_EXISTING;
        DWORD dwDesiredAccess = 0;
        DWORD dwShareMode = 0;

        dwDesiredAccess |= (a_Flags & OpenFlags_Read)  ? GENERIC_READ  : dwDesiredAccess;
        dwDesiredAccess |= (a_Flags & OpenFlags_Write) ? GENERIC_WRITE : dwDesiredAccess;

        if (a_Flags & OpenFlags_CreateNew)
        {
            dwShareMode = FILE_SHARE_READ;
            dwCreationDisposition = CREATE_ALWAYS;
            dwDesiredAccess |= GENERIC_WRITE;
        }

        m_Handle = CreateFileA(a_FileName.GetString(), dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, NULL, NULL);

        if (m_Handle != INVALID_HANDLE_VALUE)
        {
            m_Position = 0;
            m_RBufferPosition = 0;
            m_PrevBufferPos = -1;
            m_LastBufferSize = 0;

            if (a_Flags & OpenFlags_NoBuffer)
            {
                m_UseBuffers = false;
            }
            else
            {
                m_RBuffer = (char*)TMalloc(BUFFER_SIZE);
                m_WBuffer = (char*)TMalloc(BUFFER_SIZE);
                m_UseBuffers = true;
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    void TNativeFile::Close()
    {
        FlushWriteBuffer();
        CloseHandle(m_Handle);

        m_Handle = INVALID_HANDLE_VALUE;
        m_Position = -1;
        m_RBufferPosition = -1;
        m_PrevBufferPos = -1;
        m_LastBufferSize = -1;

        if (m_RBuffer != TNULL)
        {
            TFree(m_RBuffer);
            m_RBuffer = TNULL;
        }

        if (m_WBuffer != TNULL)
        {
            TFree(m_WBuffer);
            m_WBuffer = TNULL;
        }
    }
}