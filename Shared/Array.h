#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <assert.h>

class CArray 
{
	public:
		// Default constructor
		CArray()
		{
			m_uiDataSize = 0;
			m_bReadOnly = false;
			m_ucData = NULL;
			m_uiWritePointer = 0;
			m_uiReadPointer = 0;
		}

		// Array constructor
		CArray(unsigned char * ucData, unsigned int uiSize, bool bCopyData = true)
		{
			// Reset the values
			m_uiDataSize = 0;
			m_bReadOnly = false;
			m_ucData = NULL;
			m_uiWritePointer = 0;
			m_uiReadPointer = 0;
			// Do we have any data?
			if(uiSize > 0)
			{
				// Do we want to copy the data?
				if(bCopyData)
				{
					// Resize our data
					Resize(uiSize);
					// Copy the data to our data
					memcpy(m_ucData, ucData, uiSize);
				}
				else
				{
					// Set our data size
					m_uiDataSize = uiSize;
					// Set our read only flag
					m_bReadOnly = true;
					// Set the data pointer
					m_ucData = ucData;
				}
				// Set our write pointer
				m_uiWritePointer = uiSize;
			}
		}

		bool            IsReadOnly() { return m_bReadOnly; }

		unsigned int    GetSize() { return m_uiDataSize; }
		unsigned char   *GetData() { return m_ucData; }

		unsigned int    GetWritePointer() { return m_uiWritePointer; }
		void			SetWritePointer(unsigned int uiWritePointer) { m_uiWritePointer = uiWritePointer; }
		void            ResetWritePointer() { m_uiWritePointer = 0; }

		unsigned int    GetReadPointer() { return m_uiReadPointer; }
		void			SetReadPointer(unsigned int uiReadPointer) { m_uiReadPointer = uiReadPointer; }
		void            ResetReadPointer() { m_uiReadPointer = 0; }

		void			Reset() 
		{
			ResetReadPointer();
			ResetWritePointer();
			m_ucData = NULL;
			m_uiDataSize = 0;
		}

		void Resize(unsigned int uiSize)
		{
			// Are we read only?
			if(m_bReadOnly)
			{
				assert(0);
				return;
			}
			if(m_uiDataSize == 0) // Allocate memory
				m_ucData = (unsigned char *)malloc(uiSize);
			else // Reallocate memory
				m_ucData = (unsigned char *)realloc(m_ucData, uiSize);

			// Set the data size
			m_uiDataSize = uiSize;
		}

		template <typename T>
		void Push(T data)
		{
			// Are we read only?
			if(m_bReadOnly)
			{
				assert(0);
				return;
			}
			// Resize our data
			Resize(m_uiDataSize + sizeof(T));
			// Copy the data to our data
			memcpy((m_ucData + m_uiWritePointer), &data, sizeof(T));
			// Increment our write pointer
			m_uiWritePointer += sizeof(T);
		}
		void Push(unsigned char * ucData, unsigned int uiSize)
		{
			// Are we read only?
			if(m_bReadOnly)
			{
				assert(0);
				return;
			}
			// Resize our data
			Resize(m_uiDataSize + uiSize);
			// Copy the data to our data
			memcpy((m_ucData + m_uiWritePointer), ucData, uiSize);
			// Increment our write pointer
			m_uiWritePointer += uiSize;
		}

		template <typename T>
		bool Pop(T& data)
		{
			// Do we not have enough data to read?
			if((m_uiReadPointer + sizeof(T)) > m_uiDataSize)
				return false;

			// Copy our data to the data
			memcpy(&data, (m_ucData + m_uiReadPointer), sizeof(T));
			// Increment our read pointer
			m_uiReadPointer += sizeof(T);
			return true;
		}

		bool Pop(unsigned char * ucData, unsigned int uiSize)
		{
			// Do we not have enough data to read?
			if((m_uiReadPointer + uiSize) > m_uiDataSize)
				return false;

			// Copy our data to the data
			memcpy(ucData, (m_ucData + m_uiReadPointer), uiSize);
			// Increment our read pointer
			m_uiReadPointer += uiSize;
			return true;
		}

	private:
		unsigned int    m_uiDataSize;
		bool            m_bReadOnly;
		unsigned char * m_ucData;
		unsigned int    m_uiWritePointer;
		unsigned int    m_uiReadPointer;
};

#endif
