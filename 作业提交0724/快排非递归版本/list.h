



  struct list_head 
  {
          struct list_head *next, *prev;
  };


  static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *newlisthead,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = newlisthead;
	newlisthead->next = next;
	newlisthead->prev = prev;
	prev->next = newlisthead;
}

static inline void list_add(struct list_head *newlisthead, struct list_head *head)
{
	__list_add(newlisthead, head, head->next);
}
static inline void list_add_tail(struct list_head *newlisthead, struct list_head *head)
{
	__list_add(newlisthead, head->prev, head);
}

//删除prev和next中间的节点
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}
//删除entry节点
static inline void __list_del_entry(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}
//static inline void list_del(struct list_head *entry)
//{
//	__list_del(entry->prev, entry->next);
//	entry->next = LIST_POISON1;
//	entry->prev = LIST_POISON2;
//}
//判断是否为空
static inline int list_if_empty(struct list_head *head)
{
	if((head->next == head) && (head->next == head))
		return 1;
	else
		return 0;
}
//获取最后一个节点
static inline struct list_head * list_get_tail(struct list_head *head)
{
	return head->prev;
}