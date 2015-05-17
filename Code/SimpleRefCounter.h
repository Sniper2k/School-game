#pragma once

class SimpleRefCounter: public IUnknown
{
public:

	virtual ~SimpleRefCounter();
	SimpleRefCounter();

	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);

private:

	// ���������� ��� ���� �� ��������� ��� ��� �������������.
	SimpleRefCounter(const SimpleRefCounter&);
	SimpleRefCounter& operator=(const SimpleRefCounter&);

	ULONG m_ref_count;
};
