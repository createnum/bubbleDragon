//////////////////////////////////////////////////////////////////////////
//树状节点，类似XML生成树
//
//////////////////////////////////////////////////////////////////////
#ifndef NEWBEE_UTILS_TREENODE_H
#define NEWBEE_UTILS_TREENODE_H
#include <assert.h>

namespace NEWBEE_UTILS{
template <class T>
class TreeNode
{
public:
	TreeNode* parent()							
	{
		return m_parent; 
	}
	const TreeNode* parent() const				
	{
		return m_parent; 
	}

	const TreeNode* FirstChild()	const		{ return firstChild; }	///< The first child of this node. Will be null if there are no children.
	TreeNode* FirstChild()						{ return firstChild; }
	const TreeNode* LastChild() const	{ return lastChild; }		/// The last child of this node. Will be null if there are no children.
	TreeNode* LastChild()	{ return lastChild; }

	TreeNode* IterateChildren( const TreeNode* previous ) {
		return const_cast< TreeNode* >( (const_cast< const TreeNode* >(this))->IterateChildren( previous ) );
	}

	/** Add a new node related to this. Adds a child past the LastChild.
	Returns a pointer to the new object or NULL if an error occured.
	*/
	TreeNode* InsertEndChild( const TreeNode& addThis )
	{
		TreeNode* node = addThis.Clone();
		if ( !node )
			return 0;

		return LinkEndChild( node );
	}

	/** Add a new node related to this. Adds a child past the LastChild.

	NOTE: the node to be added is passed by pointer, and will be
	henceforth owned (and deleted) by tinyXml. This method is efficient
	and avoids an extra copy, but should be used with care as it
	uses a different memory model than the other insert functions.

	@sa InsertEndChild
	*/
	TreeNode* LinkEndChild( TreeNode* node )
	{
		assert( node->m_parent == 0 || node->m_parent == this );
		node->m_parent = this;

		node->prev = lastChild;
		node->next = 0;

		if ( lastChild )
			lastChild->next = node;
		else
			firstChild = node;			// it was an empty list.

		lastChild = node;
		return node;
	}
	/** Add a new node related to this. Adds a child before the specified child.
	Returns a pointer to the new object or NULL if an error occured.
	*/
	TreeNode* InsertBeforeChild( TreeNode* beforeThis, const TreeNode& addThis )
	{	
		if ( !beforeThis || beforeThis->m_parent != this ) {
			return 0;
		}
		TreeNode* node = addThis.Clone();
		if ( !node )
			return 0;
		node->m_parent = this;

		node->next = beforeThis;
		node->prev = beforeThis->prev;
		if ( beforeThis->prev )
		{
			beforeThis->prev->next = node;
		}
		else
		{
			assert( firstChild == beforeThis );
			firstChild = node;
		}
		beforeThis->prev = node;
		return node;
	}
	/** Add a new node related to this. Adds a child after the specified child.
	Returns a pointer to the new object or NULL if an error occured.
	*/
	TreeNode* InsertAfterChild( TreeNode* afterThis, const TreeNode& addThis )
	{
		if ( !afterThis || afterThis->m_parent != this ) {
			return 0;
		}

		TreeNode* node = addThis.Clone();
		if ( !node )
			return 0;
		node->m_parent = this;

		node->prev = afterThis;
		node->next = afterThis->next;
		if ( afterThis->next )
		{
			afterThis->next->prev = node;
		}
		else
		{
			assert( lastChild == afterThis );
			lastChild = node;
		}
		afterThis->next = node;
		return node;
	}
	/** Replace a child of this node.
	Returns a pointer to the new object or NULL if an error occured.
	*/
	TreeNode* ReplaceChild( TreeNode* replaceThis, const TreeNode& withThis )
	{
		if ( replaceThis->m_parent != this )
			return 0;

		TreeNode* node = withThis.Clone();
		if ( !node )
			return 0;

		node->next = replaceThis->next;
		node->prev = replaceThis->prev;

		if ( replaceThis->next )
			replaceThis->next->prev = node;
		else
			lastChild = node;

		if ( replaceThis->prev )
			replaceThis->prev->next = node;
		else
			firstChild = node;

		delete replaceThis;
		node->m_parent = this;
		return node;
	}
	TreeNode()
	{
		m_parent = 0;
		firstChild = 0;
		lastChild = 0;
		prev = 0;
		next = 0;
	}
	virtual ~TreeNode()
	{
		//TreeNode* node = firstChild;
		//TreeNode* temp = 0;

		//while ( node )
		//{
		//	temp = node;
		//	node = node->next;
		//	delete temp;
		//}	
	}
	void Clear()//删除所有子节点。
	{
		TreeNode* node = firstChild;
		TreeNode* temp = 0;

		while ( node )
		{
			temp = node;
			node = node->next;
			delete temp;
		}	

		firstChild = 0;
		lastChild = 0;
	}
	TreeNode* Clone() const 
	{
		return 0;
	}
	/// Delete a child of this node.
	bool RemoveChild( TreeNode* removeThis )
	{
		if ( removeThis->m_parent != this )
		{	
			assert( 0 );
			return false;
		}

		if ( removeThis->next )
			removeThis->next->prev = removeThis->prev;
		else
			lastChild = removeThis->prev;

		if ( removeThis->prev )
			removeThis->prev->next = removeThis->next;
		else
			firstChild = removeThis->next;

		delete removeThis;
		return true;
	}

	const TreeNode* IterateChildren( const TreeNode* previous ) const
	{
		if ( !previous )
		{
			return FirstChild();
		}
		else
		{
			assert( previous->m_parent == this );
			return previous->NextSibling();
		}
	}
	/// Navigate to a sibling node.
	const TreeNode* PreviousSibling() const			
	{ 
		return prev; 
	}
	TreeNode* PreviousSibling()						
	{
		return prev; 
	}

	/// Navigate to a sibling node.
	const TreeNode* PreviousSibling( const char * ) const;
	TreeNode* PreviousSibling( const char *_prev ) {
		return const_cast< TreeNode* >( (const_cast< const TreeNode* >(this))->PreviousSibling( _prev ) );
	}
	/// Navigate to a sibling node.
	const TreeNode* NextSibling() const				
	{
		return next; 
	}
	TreeNode* NextSibling()							
	{
		return next; 
	}
	bool hasChildren() const
	{
		return 0 != firstChild;
	}

	/// Navigate to a sibling node with the given 'value'.
	const TreeNode* NextSibling( const char * ) const;
	TreeNode* NextSibling( const char* _next ) {
		return const_cast< TreeNode* >( (const_cast< const TreeNode* >(this))->NextSibling( _next ) );
	}
	/** Create an exact duplicate of this node and return it. The memory must be deleted
	by the caller. 
	*/
	T *getDataPointer()
	{
		return &m_data;
	}
	T& getData()
	{
		return m_data;
	}
	void setData(T& data)
	{
		m_data = data;
	}
	void releaseTree()
	{
		TreeNode* child = this->FirstChild();
		TreeNode* deleted = 0;

		while ( child )
		{
			deleted = child;
			child = child->NextSibling();
			deleted->releaseTree();
		}
		firstChild = 0;
		lastChild = 0;
		delete this;
	}
	void releaseTreewithDataPointer()
	{
		for(TreeNode* p = this->FirstChild(); p; p = p->NextSibling())
		{
			p->releaseTreewithDataPointer();
		}
		delete m_data;
		delete this;
	}

protected:

	// Copy to the allocated object. Shared functionality between Clone, Copy constructor,
	// and the assignment operator.
	//void CopyTo( TreeNode* target ) const;

	TreeNode*		m_parent;
	TreeNode*		firstChild;
	TreeNode*		lastChild;
	TreeNode*		prev;
	TreeNode*		next;
	T	m_data;
private:
	TreeNode( const TreeNode& );				// not implemented.
	void operator=( const TreeNode& base );	// not allowed.
};
}
#endif //NEWBEE_UTILS_TREENODE_H


