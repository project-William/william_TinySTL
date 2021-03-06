#pragma once
#include "allocator.h"

namespace will::tinySTL
{
	template<class T>
	struct ForwardListNode
	{
		T elem;
		ForwardListNode* m_NextPtr;
	};


	template<class T>
	class FwdListIterator
	{
	public:
		using Iterator = FwdListIterator<T>;

		FwdListIterator() :Ptr(nullptr) {}
		FwdListIterator(ForwardListNode<T>* x) :Ptr(x) {}
		Iterator& operator++() {
			Ptr = Ptr->m_NextPtr;
			return *this;
		}
		Iterator operator++(int) {
			Iterator tmp = *this;
			++* this;
			return tmp;
		}
		
		Iterator& operator=(const Iterator& x) {
			Ptr = x.Ptr;
			return *this;
		}
		T operator*() {
			return Ptr->elem;
		}
		bool operator==(const Iterator& x) {
			return (Ptr == x.Ptr);
		}
		bool operator!=(const Iterator& x) {
			return (Ptr != x.Ptr);
		}
		ForwardListNode<T>* Ptr;

	};



	template<class T, typename allocator=Allocator<ForwardListNode<T>>>
	class Forward_List
	{
	public:
		using Node = ForwardListNode<T>;
		Forward_List();
		~Forward_List();

		using iterator = FwdListIterator<T>;
		using const_iterator = FwdListIterator<T>;


		void push_back(const T& ele);
		void push_front(const T& ele);
		void pop_back();
		void pop_front();
		void delNodeByEle(const T& ele);
		void delNodeByIndex(size_t index);

		void size()const { return m_Size; }

		iterator begin() const { return m_Head; }
		const_iterator cbegin()const { return m_Head; }

		iterator end()const { return m_Tail->m_NextPtr; }
		const_iterator cend()const { return m_Tail->m_NextPtr; }

		iterator getLastNode()const { return m_Tail; }



	private:
		Node* m_Head;
		Node* m_Tail;
		allocator m_Alloc;
		size_t m_Size;
	};


	template<class T, typename allocator>
	Forward_List<T, allocator>::Forward_List()
		:m_Head(nullptr),m_Tail(nullptr), m_Size(0)
	{

	}

	template<class T, typename allocator>
	Forward_List<T, allocator>::~Forward_List()
	{
		Node* ptr = m_Head;
		Node* nodeToFree = m_Head;
		while (ptr)
		{
			nodeToFree = ptr;
			ptr = ptr->m_NextPtr;
			m_Alloc.deallocate(nodeToFree);
			nodeToFree = nullptr;
		}
		m_Tail = m_Head = nullptr;
	}

	template<class T, typename allocator>
	void Forward_List<T, allocator>::push_back(const T& ele)
	{
		if (!m_Head)
		{
			m_Head = m_Alloc.allocate(1);
			m_Head->elem = ele;
			m_Head->m_NextPtr = nullptr;
			m_Tail = m_Head;
		}
		else
		{
			Node* newNode = m_Alloc.allocate(1);
			newNode->elem = ele;
			m_Tail->m_NextPtr = newNode;
			m_Tail = newNode;
		}
		m_Tail->m_NextPtr = nullptr;
		m_Size++;
	}

	template<class T,typename allocator>
	void Forward_List<T, allocator>::push_front(const T& ele)
	{
		if (!m_Head)
		{
			m_Head = m_Alloc.allocate(1);
			m_Head->elem = ele;
			m_Head->m_NextPtr = nullptr;
			m_Tail = m_Head;
		}
		else
		{
			Node* newNode = m_Alloc.allocate(1);
			newNode->elem = ele;
			newNode->m_NextPtr = m_Head;
			m_Head = newNode;
			Node* tmp = m_Head;
			while (tmp->m_NextPtr)
			{
				tmp = tmp->m_NextPtr;
			}
			m_Tail = tmp;
		}
		m_Tail->m_NextPtr = nullptr;
		m_Size++;
	}


	template<class T,typename allocator>
	void Forward_List<T, allocator>::pop_back()
	{
		Node* tmp = m_Head;
		if (tmp == m_Tail)
		{
			m_Alloc.deallocate(tmp);
			m_Tail = m_Head = nullptr;
		}
		else
		{
			while (tmp)
			{
				if (tmp->m_NextPtr == m_Tail)
					break;
				else
				{
					tmp = tmp->m_NextPtr;
				}
			}
			m_Alloc.deallocate(m_Tail);
			m_Tail = tmp;
		}
		m_Tail->m_NextPtr = nullptr;
		m_Size--;
	}

	template<class T,typename allocator>
	void Forward_List<T, allocator>::pop_front()
	{
		Node* nodeToFree = m_Head;
		if (!m_Head)
		{
			throw std::exception("error! Empty forward list!");
		}
		else
		{
			m_Head = m_Head->m_NextPtr;
			m_Alloc.deallocate(nodeToFree);
		}
		m_Size--;
	}


	template<class T,typename allocator>
	void Forward_List<T, allocator>::delNodeByEle(const T& ele)
	{
		Node* NodeToFree = m_Head;
		Node* preNodeToFree = m_Head;
		if (!m_Head)
		{
			throw std::exception("error! Empty forward list!");
		}
		else
		{
			if (ele == m_Head->elem)
			{
				pop_front();
			}
			else if (ele == m_Tail->elem)
			{
				pop_back();
			}
			else
			{
				while (preNodeToFree && preNodeToFree->m_NextPtr)
				{
					if (preNodeToFree->m_NextPtr->elem == ele)
					{
						NodeToFree = preNodeToFree->m_NextPtr;
						preNodeToFree->m_NextPtr = NodeToFree->m_NextPtr;
						m_Alloc.deallocate(NodeToFree);

					}
					else preNodeToFree = preNodeToFree->m_NextPtr;
				}
			}
			
		}
		m_Size--;
	}

	template<class T,typename allocator>
	void Forward_List<T, allocator>::delNodeByIndex(size_t index)
	{
		if (index >= m_Size || index < 0)
		{
			throw std::exception("index is outlier");
		}
		else
		{
			if (!m_Head)
				throw std::exception("empty forward list");
			else
			{
				Node* preNodeToFree = m_Head;
				if (index == 0)
				{
					Node* NodeToFree = m_Head;
					m_Head = m_Head->m_NextPtr;
					m_Alloc.deallocate(NodeToFree);
				}
				else
				{
					int i = 0;
					while (preNodeToFree)
					{
						if (i == index - 1)
							break;
						i++;
						preNodeToFree = preNodeToFree->m_NextPtr;
					}
					Node* NodeToFree = preNodeToFree->m_NextPtr;
					if (NodeToFree == m_Tail)
						m_Tail = preNodeToFree;
					preNodeToFree->m_NextPtr = NodeToFree->m_NextPtr;
					m_Alloc.deallocate(NodeToFree);
				}
			}
		}
		m_Size--;
	}

}


