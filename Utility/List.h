#ifndef __UTILITY_LIST_H__
#define __UTILITY_LIST_H__

#include "Utility.h"

namespace Utility { namespace Container
{

/// <summary>
/// A standard list of nodes. It manages an item from the template parameter class and has a pointer that links to the tail list node, if it exists,
/// or is the <code>NULL</code> pointer if the node is at the end of the list.
/// </summary>
/// <remarks>
/// Typically, a list should not have a lot of nodes. If that is the case other types of containers may be more efficient.
/// </remarks>
template <class T>
class List
{
/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary>
	/// The list constructor allows you to specify the root object and the tail of the list.
	/// </summary>
	explicit List(const T& item, List<T>* tailList = NULL);
	
	/// <summary>
	/// The list does not implement a recursive destructor that deletes the front node of the list and then asks the rest of the list to delete itself.
	/// A recursive destructor is problematic for very long lists because it may cause an overflow of the program stack.
	/// The user is responsible for destroying the list, e.g. by performing following steps:
	/// <code>
	/// while (list)
	/// {
	///		List<int>* frontList = list;
	///		list = list->GetTail();
	///		delete frontList;
	/// }
	/// </code>
	/// </summary>
	~List();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void SetItem(const T& item);
	T& GetItem();
	const T& GetItem() const;
	void SetTail(List<T>* tailList);
	List<T>* GetTail();
	const List<T>* GetTail() const;
	
	/// <summary>
	/// Returns the number of nodes in the list. Note that the list does not maintain a number of nodes in the list, so it is inefficient to call this function often.
	/// </summary>
	int GetSize() const;

	/// <summary>
	/// Removes the front element from the tail list. If current node has item A and tail list with nodes [B, C, D, E]
	/// (the whole list can be written as [A, B, C, D, E]) the operation will remove node B from the tail list. The final list will be [A, C, D, E].
	/// </summary>
	void RemoveTailFront();

	/// <summary>
	/// Removes the node (or all nodes if <paramref name="removeAllMatchingNodesEnabled"/> is set to <code>true</code>) from the tail list which contains the given <paramref name="itemToRemove"/>.
	/// If no such node is found then nothing is changed in the list.
	/// If <paramref name="removeAllMatchingNodesEnabled"/> is set to <code>true</code> then all nodes with the item equal to the <paramref name="itemToRemove"/> are removed.
	/// Otherwise, if <paramref name="removeAllMatchingNodesEnabled"/> is set to <code>false</code> then only first matching node is removed.
	/// </summary>
	/// <remarks>
	/// Current node is not checked if it contains a matching item. Thus, if current node contains the item which match the given <paramref name="itemToRemove"/> it will not be removed anyway.
	/// </remarks>
	void RemoveTailItem(const T& itemToRemove, bool removeAllMatchingNodesEnabled);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	T& m_item;
	List<T>* m_tailList;
/* ==================== Non-static member variables end ==================== */
}; /* end class List */

} /* end namespace Container */

} /* end namespace Utility */

#endif /* __UTILITY_LIST_H__ */