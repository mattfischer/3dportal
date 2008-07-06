#ifndef U_COLLECTION_H
#define U_COLLECTION_H

#include <windows.h>
#include <string>

using std::string;

class U_CollectionItem
{
public:
	virtual void SetNum( int n )
	{

	}

	virtual int GetNum()
	{
		return -1;
	}

	virtual void SetName( const string& s )
	{

	}

	virtual const string& GetName()
	{
		static const string empty;
		return empty;
	}


	virtual void Acquire()
	{

	}

	virtual void Release()
	{

	}

	virtual void SetPendingDelete()
	{
	}

	virtual bool GetPendingDelete()
	{
		return false;
	}

	virtual int GetRefCount()
	{
		return 0;
	}
};

class U_NumerableItem : virtual public U_CollectionItem
{
public:
	void SetNum( int n )
	{
		num = n;
	}

	int GetNum()
	{
		return num;
	}

protected:
	int num;
};

class U_NameableItem : virtual public U_CollectionItem
{
public:
	U_NameableItem()
	{
	}

	U_NameableItem( U_NameableItem &i )
	{
		name = i.name;
	}

	void SetName( const string& s )
	{
		name = s;
	}

	const string& GetName()
	{
		return name;
	}

	virtual ~U_NameableItem()
	{
	}

protected:
	string name;
};

class U_RefCountedItem : virtual public U_CollectionItem
{
public:
	U_RefCountedItem()
	{
		refCount = 0;
		pendingDelete = false;
	}

	void Acquire()
	{
		refCount++;
	}

	void Release()
	{
		refCount--;
	}

	virtual ~U_RefCountedItem()
	{
	}

	int GetRefCount()
	{
		return refCount;
	}

	bool GetPendingDelete()
	{
		return pendingDelete;
	}

	void SetPendingDelete()
	{
		pendingDelete = true;
	}

protected:
	int refCount;
	bool pendingDelete;
};

template<typename T> class U_Collection
{
public:
	U_Collection();
	~U_Collection();

	void Allocate( int n );

	T *operator[]( const string& n );
	T *operator[]( int n );

	T *Acquire( int n );
	void Release( int n );

	int Add( T *t );
	int Index( const string& n );

	void Remove( int n );
	int NumItems();

protected:

	T **items;
	int numItems;
	CRITICAL_SECTION critSec;
};

template<typename T> U_Collection<T>::U_Collection()
{
	items = NULL;
	numItems = 0;
	InitializeCriticalSection( &critSec );
}

template<typename T> U_Collection<T>::~U_Collection()
{
	delete[] items;
	DeleteCriticalSection( &critSec );
}

template<typename T> void U_Collection<T>::Allocate( int n )
{
	int i;

	numItems = n;
	items = new T*[numItems];
	for( i = 0 ; i < numItems ; i++ )
		items[i] = NULL;
}

template<typename T> T *U_Collection<T>::operator[]( int n )
{
	return items[n];
}

template<typename T> T *U_Collection<T>::Acquire( int n )
{
	EnterCriticalSection( &critSec );
	if( items[n] != NULL )
	{
		if( items[n]->GetPendingDelete() )
		{
			LeaveCriticalSection( &critSec );
			return NULL;
		}
		else
			items[n]->Acquire();
	}
	LeaveCriticalSection( &critSec );
	return items[n];
}

template<typename T> void U_Collection<T>::Release( int n )
{
	T *toDelete;

	toDelete = NULL;
	EnterCriticalSection( &critSec );
	if( items[n] != NULL )
	{
		items[n]->Release();
		if( items[n]->GetPendingDelete() && items[n]->GetRefCount() == 0 )
		{
			toDelete = items[n];
			items[n] = NULL;
		}
	}
	LeaveCriticalSection( &critSec );

	if( toDelete != NULL )
		delete toDelete;
}

template<typename T> T *U_Collection<T>::operator[]( const string& n )
{
	int i;

	for( i = 0 ; i < numItems ; i++ )
		if( items[i] != NULL )
			if( !strcmpi( items[i]->GetName().c_str(), n.c_str() ) )
				return items[i];
	return (T*)NULL;
}

template<typename T> int U_Collection<T>::Add( T *t )
{
	int i;
	for( i = 0 ; i < numItems ; i++ )
		if( items[i] == NULL)
		{
			items[i] = t;
			items[i]->SetNum(i);
			return i;
		}

	return -1;
}

template<class T> int U_Collection<T>::NumItems()
{
	return numItems;
}

template<typename T> int U_Collection<T>::Index( const string& n )
{
	int i;

	for( i = 0 ; i < numItems ; i++ )
		if( items[i] != NULL )
			if( !strcmpi( items[i]->GetName().c_str(), n.c_str() ) )
				return i;

	return -1;
}

template<class T> void U_Collection<T>::Remove( int n )
{
	T *toDelete;

	toDelete = NULL;
	EnterCriticalSection( &critSec );
	if( items[n] != NULL )
	{
		if( items[n]->GetRefCount() > 0 )
			items[n]->SetPendingDelete();
		else
		{
			toDelete = items[n];
			items[n] = NULL;
		}
	}
	LeaveCriticalSection( &critSec );

	if( toDelete != NULL ) 
		delete toDelete;
}
#endif

