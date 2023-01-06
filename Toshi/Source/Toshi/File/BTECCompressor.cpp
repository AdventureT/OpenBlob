#include "ToshiPCH.h"
#include "BTECCompressor.h"

namespace Toshi
{
    BTECCompressor::BTECCompressor()
    {
        this->m_Buffer1 = TNULL;
        this->m_DataEnd = TNULL;
        this->m_Buffer2 = TNULL;
        this->m_Buffer3 = TNULL;
        this->m_SomeArray1 = TNULL;
        this->m_Buckets = TNULL;
        this->m_BucketCount = 0;
    }

    BTECCompressor::~BTECCompressor()
    {
        //TFree(m_SomeArray1);
    }

    void BTECCompressor::Initialize(char* buffer, size_t bufferSize, int maxoffset, int unk)
    {
        // 0068ac60
        m_DataEnd = buffer + bufferSize;
        m_Buffer1 = buffer;
        m_Buffer2 = buffer;
        m_Buffer3 = buffer;
        m_MaxOffset = maxoffset;
        m_BucketCount = (unk < 1) ? 0 : unk - 1;

        m_Root1.Reset();
        m_Root2.Reset();

        size_t arraySize1 = m_BucketCount * maxoffset;
        m_SomeArray1 = (Node*)TMemalign(16, arraySize1 * sizeof(Node));

        for (size_t i = 0; i < arraySize1; i++)
        {
            m_SomeArray1[i].m_Ptr1 = &m_Root1;
            m_SomeArray1[i].m_Ptr2 = m_Root1.m_Ptr2;
            m_SomeArray1[i].m_Ptr1->m_Ptr2 = &m_SomeArray1[i];
            m_SomeArray1[i].m_Ptr2->m_Ptr1 = &m_SomeArray1[i];
        }

        TUtil::MemSet(m_Offsets, '\0', sizeof(m_Offsets));

        m_Buckets = (Node***)TMemalign(16, m_BucketCount * sizeof(Node**));

        for (size_t i = 0; i < m_BucketCount; i++)
        {
            m_Buckets[i] = (Node**)TMemalign(16, 256 * sizeof(Node*));

            for (size_t k = 0; k < 256; k++)
            {
                Node* someNode = new Node();
                someNode->m_Unk1 = someNode;
                someNode->m_Unk2 = someNode;
                m_Buckets[i][k] = someNode;
            }
        }
    }

    void BTECCompressor::FUN_0068ae40(size_t dataSize)
    {
        // 0068ae40
        auto oldBuffer3 = m_Buffer3;
        auto newBuffer3 = m_Buffer3 + dataSize;

        if (m_DataEnd <= newBuffer3 && newBuffer3 != m_Buffer3)
        {
            newBuffer3 = m_DataEnd;
        }

        m_Buffer3 = newBuffer3;

        if (m_MaxOffset < m_Buffer3 - m_Buffer2)
        {
            m_Buffer2 = m_Buffer3 - m_MaxOffset;

            for (size_t i = 0; i < 256; i++)
            {
                if (m_Offsets[i] < m_Buffer2)
                {
                    m_Offsets[i] = 0;
                }
            }

            auto node = m_Root2.m_Ptr1;
            while (node != &m_Root2)
            {
                if (m_Buffer2 <= node->m_Unk3) break;
                auto nextNode = node->m_Ptr1;
                
                node->m_Unk1->m_Unk2 = node->m_Unk2;
                node->m_Unk2->m_Unk1 = node->m_Unk1;
                node->m_Unk1 = node;
                node->m_Unk2 = node;
                node->m_Ptr1->m_Ptr2 = node->m_Ptr2;
                node->m_Ptr2->m_Ptr1 = node->m_Ptr1;
                node->m_Ptr1 = &m_Root1;
                node->m_Ptr2 = m_Root1.m_Ptr2;
                m_Root1.m_Ptr2 = node;
                node->m_Ptr2->m_Ptr1 = node;

                node = nextNode;
            }
        }

        while (oldBuffer3 < m_Buffer3)
        {
            AllocSubstring(oldBuffer3);
            oldBuffer3++;
        }
    }

    bool BTECCompressor::FUN_0068af10(char* buffer, size_t bufferSize, char*& offset, size_t& dataSize)
    {
        offset = 0;
        dataSize = 0;

        if (bufferSize < 1 || m_Offsets[*buffer] == TNULL)
        {
            return false;
        }

        if (m_BucketCount > 0)
        {
            size_t size = 0;
            Node* node1 = TNULL;
            Node* node2 = TNULL;
            Bucket bucket;

            for (size_t i = 0; i < m_BucketCount; i++)
            {
                size = 2 << (i % 32);
                if (bufferSize < size) break;
                bucket = m_Buckets[i];
                
                bool bRes = FUN_0068b300(buffer, bucket, size, node1, node2);
                if (bRes == false) break;
            }

            if (node1 != TNULL)
            {
                size_t someSize;

                if (bucket == m_Buckets[m_BucketCount - 1])
                {
                    someSize = TMath::Min(bufferSize, static_cast<size_t>(m_DataEnd - node1->m_Unk3));
                }
                else
                {
                    someSize = TMath::Min(bufferSize, size * 2 - 1);
                }

                if (node1 == node2)
                {
                    return true;
                }

                while (true)
                {
                    size = 0;

                    while (node1->m_Unk3[size] == buffer[size])
                    {
                        if (someSize <= size) break;
                        size++;
                    }

                    if (dataSize < size)
                    {
                        dataSize = size;
                        offset = node1->m_Unk3;

                        if (someSize <= size)
                        {
                            return true;
                        }
                    }

                    node1 = node1->m_Unk1;

                    if (node1 == node2)
                    {
                        return true;
                    }
                }
            }
        }

        offset = m_Offsets[*buffer];
        dataSize = 1;

        return true;
    }

    void BTECCompressor::AllocSubstring(char* buffer)
    {
        // 0068b180
        m_Offsets[*buffer] = buffer;
        
        for (size_t i = 0; i < m_BucketCount; i++)
        {
            auto node = m_Root1.m_Ptr1;
            size_t unknown = 2 << (i % 32);

            if (m_DataEnd - buffer < unknown)
            {
                return;
            }

            if (node == &m_Root1)
            {
                TOSHI_CORE_ERROR("SlidingWindow::AllocSubstring: No free substrings!");
                TBREAK();
                return;
            }

            node->m_Ptr1->m_Ptr2 = node->m_Ptr2;
            node->m_Ptr2->m_Ptr1 = node->m_Ptr1;
            node->m_Ptr1 = &m_Root2;
            node->m_Ptr2 = (m_Root2).m_Ptr2;
            m_Root2.m_Ptr2 = node;
            node->m_Ptr2->m_Ptr1 = node;
            node->m_Unk4 = (void*)(m_DataEnd - buffer);
            node->m_Unk3 = buffer;

            uint32_t hash = HashData(buffer, unknown);

            auto hashedNode = m_Buckets[i][hash % 256];
            node->m_Unk1 = hashedNode;
            node->m_Unk2 = hashedNode->m_Unk2;
            hashedNode->m_Unk2 = node;
            node->m_Unk2->m_Unk1 = node;
        }
    }

    bool BTECCompressor::FUN_0068b300(char* buffer, Bucket nodeBucket, size_t bufferSize, Node*& out1, Node*& out2)
    {
        auto hash = HashData(buffer, bufferSize);
        auto hashedNode = nodeBucket[hash % 256];

        out2 = hashedNode;
        out1 = hashedNode->m_Unk1;

        while (out1 != out2)
        {
            int difference = T2String8::Compare((const char*)out1->m_Unk3, buffer, bufferSize);
            if (difference == 0) break;

            out1 = out1->m_Unk1;
        }

        return out1 != out2;
    }
}