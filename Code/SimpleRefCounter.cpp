#include "StdAfx.h"
#include "SimpleRefCounter.h"

SimpleRefCounter::SimpleRefCounter()
:m_ref_count(0)
{
	AddRef();
}

SimpleRefCounter::~SimpleRefCounter()
{
}

ULONG STDMETHODCALLTYPE SimpleRefCounter::AddRef(void)
{
	m_ref_count++;
	return m_ref_count;
}

ULONG STDMETHODCALLTYPE SimpleRefCounter::Release(void)
{
	m_ref_count--;
	if( 0 < m_ref_count )
		return m_ref_count;
	delete this;
	return 0;
}

HRESULT STDMETHODCALLTYPE SimpleRefCounter::QueryInterface(REFIID iid, void ** ppvObject)
{
	return E_NOTIMPL;
}
