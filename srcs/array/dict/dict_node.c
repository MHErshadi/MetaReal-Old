// MetaReal Programming Language version 1.0.0

#include <ir/block.h>
#include <stddef.h>

uint8 dict_node_exist(dict_node_p node, dict_node_p element);

dict_node_p set_dict_node(heap_t heap, block_t key, block_t value, dict_node_p left, dict_node_p right)
{
    dict_node_p node = heap_fixed_alloc(heap, sizeof(dict_node_t));

    node->_key = key;
    node->_value = value;
    node->_left = left;
    node->_right = right;

    return node;
}

dict_node_p copy_dict_node(stack_t stack, heap_t heap, dict_node_p src)
{
    dict_node_p dst = heap_fixed_alloc(heap, sizeof(dict_node_t));

    dst->_key = block_copy(src->_key, stack, heap);
    dst->_value = block_copy(src->_value, stack, heap);

    if (src->_left)
        dst->_left = copy_dict_node(stack, heap, src->_left);
    else
        dst->_left = NULL;

    if (src->_right)
        dst->_right = copy_dict_node(stack, heap, src->_right);
    else
        dst->_right = NULL;

    return dst;
}

void free_dict_node(heap_t heap, dict_node_p node)
{
    if (node->_left)
        free_dict_node(heap, node->_left);
    if (node->_right)
        free_dict_node(heap, node->_right);

    block_free(heap, node->_key);
    block_free(heap, node->_value);

    heap_fixed_free(heap, node, sizeof(dict_node_t));
}

uint8 dict_node_new(heap_t heap, dict_node_p node, block_t key, block_t value)
{
    uint64 key_weight = block_weight(key);

    while (1)
    {
        uint64 node_weight = block_weight(node->_key);

        if (node->_key._type != key._type || node_weight != key_weight)
        {
            if (key_weight < node_weight)
            {
                if (!node->_left)
                {
                    node->_left = set_dict_node(heap, key, value, NULL, NULL);
                    return 1;
                }

                node = node->_left;
                continue;
            }

            if (!node->_right)
            {
                node->_right = set_dict_node(heap, key, value, NULL, NULL);
                return 1;
            }

            node = node->_right;
            continue;
        }

        if (block_equal(node->_key, key))
        {
            block_free(heap, key);
            block_free(heap, node->_value);

            node->_value = value;

            return 0;
        }

        if (!node->_right)
        {
            node->_right = set_dict_node(heap, key, value, NULL, NULL);
            return 1;
        }
        node = node->_right;
    }
}

uint8 dict_node_add(heap_t heap, dict_node_p node, dict_node_p add)
{
    uint64 add_weight = block_weight(add->_key);

    while (1)
    {
        uint64 node_weight = block_weight(node->_key);

        if (node->_key._type != add->_key._type || node_weight != add_weight)
        {
            if (add_weight < node_weight)
            {
                if (!node->_left)
                {
                    node->_left = add;
                    return 1;
                }

                node = node->_left;
                continue;
            }

            if (!node->_right)
            {
                node->_right = add;
                return 1;
            }

            node = node->_right;
            continue;
        }

        if (block_equal(node->_key, add->_key))
        {
            block_free(heap, add->_key);
            block_free(heap, node->_value);

            node->_value = add->_value;

            return 0;
        }

        if (!node->_right)
        {
            node->_right = add;
            return 1;
        }
        node = node->_right;
    }
}

uint64 dict_node_concat(heap_t heap, dict_node_p dst, dict_node_p src)
{
    uint64 add = 0;

    dict_node_p left = src->_left;
    dict_node_p right = src->_right;

    src->_left = NULL;
    src->_right = NULL;
    add += dict_node_add(heap, dst, src);

    if (left)
        add += dict_node_concat(heap, dst, left);
    if (right)
        add += dict_node_concat(heap, dst, right);

    return add;
}

uint8 dict_node_contains(dict_node_p node, block_t key)
{
    uint64 key_weight = block_weight(key);

    do
    {
        uint64 node_weight = block_weight(node->_key);

        if (node->_key._type != key._type || node_weight != key_weight)
        {
            node = key_weight < node_weight ? node->_left : node->_right;
            continue;
        }

        if (block_equal(node->_key, key))
            return 1;

        node = node->_right;
    } while (node);

    return 0;
}

uint8 dict_node_equal(dict_node_p op1, dict_node_p op2)
{
    if (!dict_node_exist(op1, op2))
        return 0;

    if (op2->_left && !dict_node_equal(op1, op2->_left))
        return 0;

    if (op2->_right && !dict_node_equal(op1, op2->_right))
        return 0;

    return 1;
}

uint8 dict_node_exist(dict_node_p node, dict_node_p element)
{
    uint64 key_weight = block_weight(element->_key);

    do
    {
        uint64 node_weight = block_weight(node->_key);

        if (node->_key._type != element->_key._type || node_weight != key_weight)
        {
            node = key_weight < node_weight ? node->_left : node->_right;
            continue;
        }

        if (block_equal(node->_key, element->_key))
            return block_equal(node->_value, element->_value);

        node = node->_right;
    } while (node);

    return 0;
}
