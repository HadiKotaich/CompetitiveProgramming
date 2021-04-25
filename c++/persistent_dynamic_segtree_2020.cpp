struct node;
extern node* empty;
struct node{
	int freq;
	node *l,*r;
	node()
	{
		freq=0;
		l=r=this;
	}
	node(int f,node *lft=empty,node *rt=empty)
	{
		freq=f;
		l=lft;
		r=rt;
	}
};
node *empty=new node;
vector<node*> roots,parents;
node *insert(node *root,int val,int nst=INT_MIN,int nen=INT_MAX){
	if(val<nst||val>nen)
		return root;
	if(nst==nen)
		return new node(root->freq+1);
	int md=nst+(nen-0LL-nst)/2;

	node *rt1=insert(root->l,val,nst,md);
	node *rt2=insert(root->r,val,md+1,nen);
	return new node(rt1->freq+rt2->freq,rt1,rt2);
}

// special query to get the kth minimum node on a path
int query(node *rtu,node *rtv,node *rtLCA,node *rtp,int k,int nst=INT_MIN,int nen=INT_MAX){
	if(nst==nen)
		return nst;
	int lsz=rtu->l->freq+rtv->l->freq-rtLCA->l->freq-rtp->l->freq;
	int md=nst+(nen-0ll-nst)/2;
	if(k<=lsz)
		return query(rtu->l,rtv->l,rtLCA->l,rtp->l,k,nst,md);
	else
		return query(rtu->r,rtv->r,rtLCA->r,rtp->r,k-lsz,md+1,nen);
}