void main()
{
   int i;
   int j;
    for (i = 2; i < 7; i = i + 1) {
    
        for (j = 2; j < 6; j = j + 1) {
            if (i * j == 10) break;
        }
        if (i == 5) break;
    }
}