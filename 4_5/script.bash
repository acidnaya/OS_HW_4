# указать сколько посетителей + указать порт вахтера и его ip
# & чтобы запустить параллельно
for i in {1..55}
do
   ./client 127.0.0.1 8888 &
done