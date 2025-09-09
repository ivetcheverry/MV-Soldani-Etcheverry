int main()
{
    int total=0, n;
    printf("Ingrese un numero natural a sumar \n",n);
    scanf("%d",&n);
    while (n!=0)
        {
        total+=n;
        printf("Ingrese un numero natural a sumar \n",n);
        scanf("%d",&n);
        }
    printf("La suma resultante es: %d",total);
    return 0;
}