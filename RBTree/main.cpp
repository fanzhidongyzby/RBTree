#include"rb_tree.h"
#include <time.h> 
#include <windows.h>

int main()
{
	srand((unsigned)GetCurrentTime());
	int times=10,len=30;
	while(times--)
	{
		rb_tree<int> tree;
		for(int i=0;i<len;i++)
		{
			int num=rand()%len;
			tree.insert_unique(num);
			if(!tree.validate())cout<<"插入时失去平衡"<<endl;
		}
		cout<<"正序：";
		for(rb_tree<int>::node_type*node=tree.minimum();node;node=tree.next(node))
		{
			cout<<node->value<<" ";
		}
		cout<<"\n旋转次数-黑高-节点数："<<tree.get_rotate_times()
			<<" "<<tree.height()<<" "<<tree.count()<<endl;
		cout<<"删除：";
		for(int i=0;i<len;i++)
		{
			int num=rand()%len;
			if(tree.remove(num))cout<<num<<" ";
			if(!tree.validate())cout<<"删除时失去平衡"<<endl;
		}
		cout<<endl;
		cout<<"逆序：";
		for(rb_tree<int>::node_type*node=tree.maximum();node;node=tree.prev(node))
		{
			cout<<node->value<<" ";
		}
		cout<<"\n旋转次数-黑高-节点数："<<tree.get_rotate_times()
			<<" "<<tree.height()<<" "<<tree.count()<<endl;
		cout<<"________________________________________________________________________________"<<endl;
	}
	return 0;
}