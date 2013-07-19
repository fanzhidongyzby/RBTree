#pragma once
#include<iostream>
using namespace std;

//������ڵ���ɫ
enum rb_tree_node_color
{
	red=false,
	black=true
};

//������ڵ�
template<class T>
class rb_tree_node
{
	typedef rb_tree_node_color node_color;
	typedef rb_tree_node<T> node_type;
public:
	node_color color;//��ɫ
	node_type*parent;//���ڵ�
	node_type*left;//���ӽڵ�
	node_type*right;//���ӽڵ�
	T value;//ֵ
	rb_tree_node(T&v);//���캯��
	inline node_type*brother();//��ȡ�ֵܽڵ�
	inline bool on_left();//���������ӽڵ�
	inline bool on_right();//���������ӽڵ�
	inline void set_left(node_type*node);//�������ӽڵ�
	inline void set_right(node_type*node);//�������ӽڵ�
};

//�����
template<class T>
class rb_tree
{
public:
	typedef rb_tree_node<T> node_type;
	rb_tree();
	~rb_tree();
	void clear();
	void insert(T v);//��ӽڵ�
	bool insert_unique(T v);//���Ψһ�ڵ�
	node_type* find(T v);//��ѯ�ڵ�
	bool  remove(T v);//ɾ���ڵ�
	inline node_type* maximum();//���ֵ
	inline node_type* minimum();//��Сֵ
	inline node_type* next(node_type*node);//��һ���ڵ�
	inline node_type* prev(node_type*node);//��һ���ڵ�
	void print();//���
	int height();//�߶�
	unsigned count();//�ڵ���
	bool validate();//��֤
	unsigned get_rotate_times();//��ȡ��ת����
private:
	node_type*root;//����
	unsigned rotate_times;//��ת�Ĵ���
	unsigned node_count;//�ڵ���
	void __clear(node_type*sub_root);//�������
	void __insert(node_type*&sub_root,node_type*parent,node_type*node);//�ڲ��ڵ���뺯��
	node_type* __find(node_type*sub_root,T v);//��ѯ
	inline node_type* __maximum(node_type*sub_root);//���ֵ
	inline node_type* __minimum(node_type*sub_root);//��Сֵ
	void __rebalance(node_type*node);//�²���ڵ����ƽ��
	void __fix(node_type*node,node_type*parent,bool direct);//ɾ���ڵ����ƽ��
	void __rotate(node_type*node);//�Զ��ж�������ת
	void __rotate_left(node_type*node);//����ת	
	void __rotate_right(node_type*node);//����ת
	void __print(node_type*sub_root);//���
	int  __height(node_type*&sub_root);//�߶�
	bool __validate(node_type*&sub_root,int& count);//��֤������ĺϷ���
};


template<class T>
rb_tree_node<T>::rb_tree_node(T&v)
	:value(v)
	,parent(NULL),left(NULL),right(NULL)
	,color(red)//Ĭ�Ͻڵ�Ϊ��ɫ��������3������·����ɫ�ڵ�����ͬ
{}

template<class T>
inline typename rb_tree_node<T>::node_type* rb_tree_node<T>::brother()
{
	if(!parent)//���ڵ�û���ֵ�
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
		return false;//�Ѿ����ڽڵ�
	insert(v);
	return true;
}

template<class T>
void rb_tree<T>::insert(T v)
{
	node_type*newNode=new node_type(v);//�����ڵ�
	if(!root)//��һ���ڵ�
	{
		root=newNode;//��¼���ڵ�
		root->color=black;//���ڵ�Ϊ��ɫ��������1�����ڵ����Ϊ��ɫ
	}
	else
	{
		__insert(root,root,newNode);//���Բ���ڵ�
		__rebalance(newNode);//���Ե���
	}
	node_count++;//�ۼƽڵ���
}

template<class T>
void rb_tree<T>::__insert(node_type*& sub_root,node_type*parent,node_type*node)
{
	if(!sub_root)//������
	{
		sub_root=node;
		node->parent=parent;
	}
	else if(node->value>=sub_root->value)//����������
	{
		__insert(sub_root->right,sub_root,node);
	}
	else//����������
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
	//���ҽڵ�
	node_type*node=find(v),*x,*y;
	if(!node)return false;
	//ȷ��ɾ����y
	if(node->left&&node->right)
	{
		y=__minimum(node->right);//������̣�ɾ������������Сֵ
		node->value=y->value;//����ԭ�ڵ�ֵ
	}
	else y=node;
	//��¼Ψһ���x	
	if(y->left)
	{
		x=y->left;
	}
	else x=y->right;
	//�ƶ�x��y�ĸ��ڵ��root
	node_type*parent=y->parent;
	bool direct=false;//Ĭ��Ϊ�ҷ�λ
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
	//��ɫ����������ƽ��
	if(y->color==black)
		__fix(x,parent,direct);
	delete y;
	node_count--;//���ٽڵ���
	return true;
}

template<class T>
void rb_tree<T>::__fix(node_type*node,node_type*parent,bool direct)
{
	while(!node||node->color==black)
	{
		if(node)parent=node->parent;//���¶�����
		if(!parent)break;//���˸��ڵ㣬����
		if(node)direct=node==node->parent->left;//node�������¶�����
		
		if(direct)//ɾ�������
		{
			node_type*w=parent->right;//��¼�ֵܽڵ�
			if(w&&w->color==red)//�ֵܺ�
			{
				w->color=black;
				parent->color=red;
				__rotate_left(parent->right);
				w=parent->right;
			}
			//ֶ��ȫ�ڣ������ֵܱغ�
			if(!w||(!w->left||w->left->color==black)&&(!w->right||w->right->color==black))
			{
				if(w)w->color=red;
				node=parent;
			}
			else
			{
				if(!w->right||w->right->color==black)//��ֶ�Ӻڣ���ֶ�Ӻ�
				{
					w->left->color=black;
					w->color=red;
					__rotate_right(w->left);
					w=parent->right;
				}
				//��ֶ�Ӻڣ���ֶ�Ӻ� ���� ֶ��ȫ��
				w->color=parent->color;
				parent->color=black;
				w->right->color=black;
				__rotate_left(parent->right);
				break;
			}
		}
		else//ɾ�����ұ�
		{
			node_type*w=parent->left;//��¼�ֵܽڵ�
			if(w&&w->color==red)//�ֵܺ�
			{
				w->color=black;
				parent->color=red;
				__rotate_right(parent->left);
				w=parent->left;
			}
			//ֶ��ȫ�ڣ������ֵܱغ�
			if(!w||(!w->left||w->left->color==black)&&(!w->right||w->right->color==black))
			{
				if(w)w->color=red;
				node=parent;
			}
			else
			{
				if(!w->left||w->left->color==black)//��ֶ�Ӻڣ���ֶ�Ӻ�
				{
					w->right->color=black;
					w->color=red;
					__rotate_left(w->right);
					w=parent->left;
				}
				//��ֶ�Ӻڣ���ֶ�Ӻ� ���� ֶ��ȫ��
				w->color=parent->color;
				parent->color=black;
				w->left->color=black;
				__rotate_right(parent->left);
				break;
			}
		}
	}
	if(node)node->color=black;//��ǰ�ڵ�ǿ��Ϊ��
}

template<class T>
typename rb_tree<T>::node_type* rb_tree<T>::__find(node_type*sub_root,T v)
{
	if(!sub_root)return NULL;
	if(v>sub_root->value)//����������
	{
		return __find(sub_root->right,v);
	}
	else if(v<sub_root->value)//����������
	{
		return __find(sub_root->left,v);
	}
	else
		return sub_root;//�ҵ����
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
		return __minimum(node->right);//�����ӽڵ㿪ʼ����Сֵ
	else
	{
		while(node->on_right())
			node=node->parent;//���ϻ���
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
		return __maximum(node->left);//�����ӽڵ㿪ʼ�����ֵ
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
	if(node->parent->color==black)//���ڵ�Ϊ��ɫ��ƽ�⡪������2��û�������ĺ�ɫ�ڵ�
		return ;
	//���ڵ�Ϊ��ɫ���϶�����root�������常�ڵ����ɫ
	node_type* uncle=node->parent->brother();//��ȡ���ڵ���ֵܽڵ�
	if(uncle&&uncle->color==red)//�常�ڵ������Ϊ��ɫ
	{
		//���������ڵ㣬����ӵ��������ɫ�ӽڵ�Ľڵ�
		for(node_type*prev=node->parent;prev!=root;prev=prev->parent)
		{
			if(prev->color==red&&prev->brother()->color==red)//����ͬΪ��ɫ���ֵ�
			{
				prev->parent->color=red;//���ø��ڵ�Ϊ��
				prev->color=black;//���Լ����ֵ���Ϊ��
				prev->brother()->color=black;			
			}
		}
		root->color=black;//ǿ�Ƹ��ڵ�Ϊ��ɫ
		//�ٴα���������������ɫ�ڵ�������ֱ���ж�prev�Ƿ�NULL����ֹ��ת����ֿ�ָ��
		for(node_type*prev=node->parent;prev;prev=prev->parent)
		{
			if(prev->color==red&&prev->parent->color==red)//�����ĺ�ɫ�ڵ�
			{
				__rotate(prev);
			}
		}
	}
	else//�常�ڵ�ΪNULL����Ϊ��ɫ������Ϊ��ɫ
	{
		__rotate(node);
	}
}

template<class T>
void rb_tree<T>::__rotate(node_type*node)
{
	//���������ڲ���뷽ʽ����ת����
	if(node->on_left()&&node->parent->on_left())
	{
		//�������룬����ת
		__rotate_right(node->parent);
		//�������ڵ������ֵܵ���ɫ
		node->parent->color=black;
		node->parent->right->color=red;

	}
	else if(node->on_right()&&node->parent->on_right())
	{
		//�������룬����ת
		__rotate_left(node->parent);
		//�������ڵ������ֵܵ���ɫ
		node->parent->color=black;
		node->parent->left->color=red;
	}
	else if(node->on_right()&&node->parent->on_left())
	{
		//���ڲ���룬����ת������ת
		__rotate_left(node);
		__rotate_right(node);
		//��������ڵ������ӽڵ����ɫ
		node->color=black;
		node->right->color=red;
	}
	else
	{
		//���ڲ���룬����ת������ת
		__rotate_right(node);
		__rotate_left(node);
		//��������ڵ������ӽڵ����ɫ
		node->color=black;
		node->left->color=red;
	}
}

template<class T>
void rb_tree<T>::__rotate_left(node_type*node)
{
	rotate_times++;
	node_type*parent=node->parent;//���ڵ�
	node_type*grand=parent->parent;//�游�ڵ�
	//������ת�ڵ�
	if(parent->on_left())grand->set_left(node);
	else if(parent->on_right())grand->set_right(node);
	else//�游�ڵ�Ϊ��
	{
		root=node;
		node->parent=NULL;
		node->color=black;
	}
	//������ת�ڵ�����ӽڵ㣬��Ϊԭ���ڵ�����ӽڵ�
	parent->set_right(node->left);
	//����ԭ���ڵ㣬��Ϊ��ת�ڵ�����ӽڵ�
	node->set_left(parent);
}

template<class T>
void rb_tree<T>::__rotate_right(node_type*node)
{
	rotate_times++;
	node_type*parent=node->parent;//���ڵ�
	node_type*grand=parent->parent;//�游�ڵ�
	//������ת�ڵ�
	if(parent->on_left())grand->set_left(node);
	else if(parent->on_right())grand->set_right(node);
	else//�游�ڵ�Ϊ��
	{
		root=node;
		node->parent=NULL;
		node->color=black;
	}
	//������ת�ڵ�����ӽڵ㣬��Ϊԭ���ڵ�����ӽڵ�
	parent->set_left(node->right);
	//����ԭ���ڵ㣬��Ϊ��ת�ڵ�����ӽڵ�
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
		count=0;//��ɫ�ڵ����
		return true;
	}
	if(sub_root->color==red&&
		((sub_root->left&&sub_root->left->color==red)
		||(sub_root->left&&sub_root->left->color==red)))
	{
		//cout<<"������ɫ"<<endl;
		return false;//���������ĺ�ɫ�ڵ㣬�������ƽ��󲻻����
	}
	int lc,rc;
	bool lf=__validate(sub_root->left,lc);
	bool rf=__validate(sub_root->right,rc);
	if(lf&&rf)//�������Ϸ����Һڸ����
	{
		if(sub_root->left&&sub_root->right&&lc!=rc)//�������������ڲűȽϸ߶�
		{
			//cout<<"�����ڸ߶Ȳ�ͬ"<<endl;
			return false;
		}
		//��֤ͨ��
		count=lc>rc?lc:rc;//���ֵ��Ϊ�ڸ�
		if(sub_root->color==black)
			count++;
		return true;
	}
	else
		return false;

}
