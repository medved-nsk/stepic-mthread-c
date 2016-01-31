

int main(int ac, char **av)
{
int flags=IPC_CREAT; // IPC_PRIVAT
key_t k=ftok();
i=msgget(k, flags);


msgctl();

return 0;

}
