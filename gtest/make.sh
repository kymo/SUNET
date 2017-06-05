g++ -isystem ./include -I./ -lpthread -c src/gtest-all.cc
ar -rv libgtest.a gtest-all.o 

