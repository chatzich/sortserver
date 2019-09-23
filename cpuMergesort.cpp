// // // // // // // // // // // // // // // //
//  CPU Implementation                       //
// // // // // // // // // // // // // // // //
void merge(int *list, int *sorted, int start, int mid, int end)
{
    int ti=start, i=start, j=mid;
    while (i<mid || j<end)
    {
        if (j==end) sorted[ti] = list[i++];
        else if (i==mid) sorted[ti] = list[j++];
        else if (list[i]<list[j]) sorted[ti] = list[i++];
        else sorted[ti] = list[j++];
        ti++;
    }

    for (ti=start; ti<end; ti++)
        list[ti] = sorted[ti];
}

void mergesort_recur(int *list, int *sorted, int start, int end)
{
    if (end-start<2)
        return;

    mergesort_recur(list, sorted, start, start + (end-start)/2);
    mergesort_recur(list, sorted, start + (end-start)/2, end);
    merge(list, sorted, start, start + (end-start)/2, end);
}

int mergesort_cpu(int *list, int *sorted, int n)
{
    mergesort_recur(list, sorted, 0, n);
    return 1;
}
