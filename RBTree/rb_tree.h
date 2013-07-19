#pragma once
#include<iostream>
using namespace std;

//红黑树节点颜色
enum rb_tree_node_color
{
	red=false,
	black=true
};

//红黑树节点
template<class T>
class rb_tree_node
{
	typedef rb_tree_node_color node_color;
	typedef rb_tree_node<T> node_type;
public:
	node_color color;//颜色
	node_type*parent;//父节点
	node_type*left;//左子节点
	node_type*right;//右子节点
	T value;//值
	rb_tree_node(T&v);//构造函数
	inline node_type*brother();//获取兄弟节点
	inline bool on_left();//自身是左子节点
	inline bool on_right();//自身是右子节点
	inline void set_left(node_type*node);//设置左子节点
	inline void set_right(node_type*node);//设置左子节点
};

//红黑树
template<class T>
class rb_tree
{
public:
	typedef rb_tree_node<T> node_type;
	rb_tree();
	~rb_tree();
	void clear();
	void insert(T v);//添加节点
	bool insert_unique(T v);//添加唯一节点
	node_type* find(T v);//查询节点
	bool  remove(T v);//删除节点
	inline node_type* maximum();//最大值
	inline node_type* minimum();//最小值
	inline node_type* next(node_type*node);//下一个节点
	inline node_type* prev(node_type*node);//上一个节点
	void print();//输出
	int height();//高度
	unsigned count();//节点数
	bool validate();//验证
	unsigned get_rotate_times();//获取旋转次数
private:
	node_type*root;//树根
	unsigned rotate_times;//旋转的次数
	unsigned node_count;//节点数
	void __clear(node_type*sub_root);//清除函数
	void __insert(node_type*&sub_root,node_type*parent,node_type*node);//内部节点插入函数
	node_type* __find(node_type*sub_root,T v);//查询
	inline node_type* __maximum(node_type*sub_root);//最大值
	inline node_type* __minimum(node_type*sub_root);//最小值
	void __rebalance(node_type*node);//新插入节点调整平衡
	void __fix(node_type*node,node_type*parent,bool direct);//删除节点调整平衡
	void __rotate(node_type*node);//自动判断类型旋转
	void __rotate_left(node_type*node);//左旋转	
	void __rotate_right(node_type*node);//右旋转
	void __print(node_type*sub_root);//输出
	int  __height(node_type*&sub_root);//高度
	bool __validate(node_type*&sub_root,int& count);//验证红黑树的合法性
};


template<class T>
rb_tree_node<T>::rb_tree_node(T&v)
	:value(v)
	,parent(NULL),left(NULL),right(NULL)
	,color(red)//默认节点为红色——规则3：任意路径黑色节点数相同
{}

template<class T>
inline typename rb_tree_node<T>::node_type* rb_tree_node<T>::brother()
{
	if(!parent)//根节点没有兄弟
		return NULL;
	else 
		return this==parent->left?parent->right:parent->left;
}

template<class T>
inline bool rb_tree_node<T>::on_left()
{
	return parent&&this==parent->left;
}

template<class T>
inline bool rb_tree_node<T>::on_right()
{
	return parent&&this==parent->right;
}

template<class T>
inline void rb_tree_node<T>::set_left(node_type*node)
{
	left=node;
	if(node)node->parent=this;
}

template<class T>
inline void rb_tree_node<T>::set_right(node_type*node)
{
	right=node;
	if(node)node->parent=this;
}

template<class T>
rb_tree<T>::rb_tree():root(NULL),rotate_times(0),node_count(0)
{
}

template<class T>
unsigned rb_tree<T>::get_rotate_times()
{
	return rotate_times;
}

template<class T>
unsigned rb_tree<T>::count()
{
	return node_count;
}


template<class T>
rb_tree<T>::~rb_tree()
{
	clear();
}

template<class T>
void rb_tree<T>::clear()
{
	__clear(root);
	root=NULL;
	rotate_times=0;
	node_count=0;
}

template<class T>
void rb_tree<T>::__clear(node_type*sub_root)
{
	if(!sub_root)return ;
	__clear(sub_root->left);
	__clear(sub_root->right);
	delete sub_root;
}

template<class T>
bool rb_tree<T>::insert_unique(T v)
{
	if(find(v))
		return false;//已经存在节点
	insert(v);
	return true;
}

template<class T>
void rb_tree<T>::insert(T v)
{
	node_type*newNode=new node_type(v);//创建节点
	if(!root)//第一个节点
	{
		root=newNode;//记录根节点
		root->color=black;//根节点为黑色——规则1：根节点必须为黑色
	}
	else
	{
		__insert(root,root,newNode);//尝试插入节点
		__rebalance(newNode);//尝试调整
	}
	node_count++;//累计节点数
}

template<class T>
void rb_tree<T>::__insert(node_type*& sub_root,node_type*parent,node_type*node)
{
	if(!sub_root)//子树空
	{
		sub_root=node;
		node->parent=parent;
	}
	else if(node->value>=sub_root->value)//插入右子树
	{
		__insert(sub_root->right,sub_root,node);
	}
	else//插入左子树
	{
		__insert(sub_root->left,sub_root,node);
	}
}

template<class T>
typename rb_tree<T>::node_type* rb_tree<T>::find(T v)
{
	return __find(root,v);
}

template<class T>
bool rb_tree<T>::remove(T v)
{
	//查找节点
	node_type*node=find(v),*x,*y;
	if(!node)return false;
	//确定删除点y
	if(node->left&&node->right)
	{
		y=__minimum(node->right);//两个后继，删除右子树的最小值
		node->value=y->value;//覆盖原节点值
	}
	else y=node;
	//记录唯一后继x	
	if(y->left)
	{
		x=y->left;
	}
	else x=y->right;
	//移动x到y的父节点或root
	node_type*parent=y->parent;
	bool direct=false;//默认为右方位
	if(y->parent)
	{
		if(direct=y->on_left())y->parent->set_left(x);
		else y->parent->set_right(x);
	}
	else 
	{
		root=x;
		if(x)x->parent=NULL;
		parent=NULL;
	}
	//颜色修正与重新平衡
	if(y->color==black)
		__fix(x,parent,direct);
	delete y;
	node_count--;//减少节点数
	return true;
}

template<class T>
void rb_tree<T>::__fix(node_type*node,node_type*parent,bool direct)
{
	while(!node||node->color==black)
	{
		if(node)parent=node->parent;//重新定父亲
		if(!parent)break;//到了根节点，结束
		if(node)direct=node==node->parent->left;//node存在重新定方向
		
		if(direct)//删除了左边
		{
			node_type*w=parent->right;//记录兄弟节点
			if(w&&w->color==red)//兄弟红
			{
				w->color=black;
				parent->color=red;
				__rotate_left(parent->right);
				w=parent->right;
			}
			//侄子全黑，这里兄弟必黑
			if(!w||(!w->left||w->left->color==black)&&(!w->right||w->right->color==black))
			{
				if(w)w->color=red;
				node=parent;
			}
			else
			{
				if(!w->right||w->right->color==black)//右侄子黑，左侄子红
				{
					w->left->color=black;
					w->color=red;
					__rotate_right(w->left);
					w=parent->right;
				}
				//左侄子黑，右侄子红 或者 侄子全红
				w->color=parent->color;
				parent->color=black;
				w->right->color=black;
				__rotate_left(parent->right);
				break;
			}
		}
		else//删除了右边
		{
			node_type*w=parent->left;//记录兄弟节点
			if(w&&w->color==red)//兄弟红
			{
				w->color=black;
				parent->color=red;
				__rotate_right(parent->left);
				w=parent->left;
			}
			//侄子全黑，这里兄弟必黑
			if(!w||(!w->left||w->left->color==black)&&(!w->right||w->right->color==black))
			{
				if(w)w->color=red;
				node=parent;
			}
			else
			{
				if(!w->left||w->left->color==black)//左侄子黑，右侄子红
				{
					w->right->color=black;
					w->color=red;
					__rotate_left(w->right);
					w=parent->left;
				}
				//右侄子黑，左侄子红 或者 侄子全红
				w->color=parent->color;
				parent->color=black;
				w->left->color=black;
				__rotate_right(parent->left);
				break;
			}
		}
	}
	if(node)node->color=black;//当前节点强制为黑
}

template<class T>
typename rb_tree<T>::node_type* rb_tree<T>::__find(node_type*sub_root,T v)
{
	if(!sub_root)return NULL;
	if(v>sub_root->value)//查找右子树
	{
		return __find(sub_root->right,v);
	}
	else if(v<sub_root->value)//查找左子树
	{
		return __find(sub_root->left,v);
	}
	else
		return sub_root;//找到结果
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::maximum()
{
	return __maximum(root);
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::minimum()
{
	return __minimum(root);
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::__maximum(node_type*sub_root)
{
	while(sub_root&&sub_root->right)
		sub_root=sub_root->right;
	return sub_root;
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::__minimum(node_type*sub_root)
{
	while(sub_root&&sub_root->left)
		sub_root=sub_root->left;
	return sub_root;
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::next(node_type*node)
{
	if(!node)return NULL;
	if(node->right)
		return __minimum(node->right);//从右子节点开始找最小值
	else
	{
		while(node->on_right())
			node=node->parent;//向上回溯
		if(node==root)
			return NULL;
		else
			return node->parent;
	}
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::prev(node_type*node)
{
	if(!node)return NULL;
	if(node->left)
		return __maximum(node->left);//从左子节点开始找最大值
	else
	{
		while(node->on_left())
			node=node->parent;
		if(node==root)
			return NULL;
		else
			return node->parent;
	}
}

template<class T>
void rb_tree<T>::__rebalance(node_type*node)
{
	if(node->parent->color==black)//父节点为黑色，平衡——规则2：没有连续的红色节点
		return ;
	//父节点为红色（肯定不是root），看叔父节点的颜色
	node_type* uncle=node->parent->brother();//获取父节点的兄弟节点
	if(uncle&&uncle->color==red)//叔父节点存在且为红色
	{
		//遍历到根节点，处理拥有两个红色子节点的节点
		for(node_type*prev=node->parent;prev!=root;prev=prev->parent)
		{
			if(prev->color==red&&prev->brother()->color==red)//遇到同为红色的兄弟
			{
				prev->parent->color=red;//设置父节点为红
				prev->color=black;//将自己和兄弟设为黑
				prev->brother()->color=black;			
			}
		}
		root->color=black;//强制根节点为黑色
		//再次遍历，处理连续红色节点的情况，直接判断prev是否NULL，防止旋转后出现空指针
		for(node_type*prev=node->parent;prev;prev=prev->parent)
		{
			if(prev->color==red&&prev->parent->color==red)//连续的红色节点
			{
				__rotate(prev);
			}
		}
	}
	else//叔父节点为NULL（视为黑色）或者为黑色
	{
		__rotate(node);
	}
}

template<class T>
void rb_tree<T>::__rotate(node_type*node)
{
	//根据外侧和内侧插入方式做旋转操作
	if(node->on_left()&&node->parent->on_left())
	{
		//左外侧插入，右旋转
		__rotate_right(node->parent);
		//交换父节点与右兄弟的颜色
		node->parent->color=black;
		node->parent->right->color=red;

	}
	else if(node->on_right()&&node->parent->on_right())
	{
		//右外侧插入，左旋转
		__rotate_left(node->parent);
		//交换父节点与左兄弟的颜色
		node->parent->color=black;
		node->parent->left->color=red;
	}
	else if(node->on_right()&&node->parent->on_left())
	{
		//左内侧插入，先左转，后右转
		__rotate_left(node);
		__rotate_right(node);
		//交换插入节点与右子节点的颜色
		node->color=black;
		node->right->color=red;
	}
	else
	{
		//右内侧插入，先右转，后左转
		__rotate_right(node);
		__rotate_left(node);
		//交换插入节点与左子节点的颜色
		node->color=black;
		node->left->color=red;
	}
}

template<class T>
void rb_tree<T>::__rotate_left(node_type*node)
{
	rotate_times++;
	node_type*parent=node->parent;//父节点
	node_type*grand=parent->parent;//祖父节点
	//处理旋转节点
	if(parent->on_left())grand->set_left(node);
	else if(parent->on_right())grand->set_right(node);
	else//祖父节点为根
	{
		root=node;
		node->parent=NULL;
		node->color=black;
	}
	//处理旋转节点的左子节点，作为原父节点的右子节点
	parent->set_right(node->left);
	//处理原父节点，作为旋转节点的左子节点
	node->set_left(parent);
}

template<class T>
void rb_tree<T>::__rotate_right(node_type*node)
{
	rotate_times++;
	node_type*parent=node->parent;//父节点
	node_type*grand=parent->parent;//祖父节点
	//处理旋转节点
	if(parent->on_left())grand->set_left(node);
	else if(parent->on_right())grand->set_right(node);
	else//祖父节点为根
	{
		root=node;
		node->parent=NULL;
		node->color=black;
	}
	//处理旋转节点的右子节点，作为原父节点的左子节点
	parent->set_left(node->right);
	//处理原父节点，作为旋转节点的右子节点
	node->set_right(parent);
}

template<class T>
void rb_tree<T>::print()
{
	__print(root);
	cout<<endl;
}

template<class T>
void rb_tree<T>::__print(node_type*sub_root)
{
	if(!sub_root)return;
	__print(sub_root->left);	
	cout<<sub_root->value<<" ";
	__print(sub_root->right);
}

template<class T>
int rb_tree<T>::height()
{
	return __height(root);
}

template<class T>
int rb_tree<T>::__height(node_type*&sub_root)
{
	if(!sub_root)return 0;
	int lh=__height(sub_root->left);
	int rh=__height(sub_root->right);
	int max=lh>rh?lh:rh;
	return max+1;
}

template<class T>
bool rb_tree<T>::validate()
{
	if(root&&root->color==red)
		return false;
	int count=0;
	return __validate(root,count);
}

template<class T>
bool rb_tree<T>::__validate(node_type*&sub_root,int& count)
{
	if(!sub_root)
	{
		count=0;//黑色节点计数
		return true;
	}
	if(sub_root->color==red&&
		((sub_root->left&&sub_root->left->color==red)
		||(sub_root->left&&sub_root->left->color==red)))
	{
		//cout<<"连续红色"<<endl;
		return false;//出现连续的红色节点，此情况在平衡后不会出现
	}
	int lc,rc;
	bool lf=__validate(sub_root->left,lc);
	bool rf=__validate(sub_root->right,rc);
	if(lf&&rf)//子树都合法，且黑高相等
	{
		if(sub_root->left&&sub_root->right&&lc!=rc)//左右子树都存在才比较高度
		{
			//cout<<"子树黑高度不同"<<endl;
			return false;
		}
		//验证通过
		count=lc>rc?lc:rc;//最大值作为黑高
		if(sub_root->color==black)
			count++;
		return true;
	}
	else
		return false;

}
