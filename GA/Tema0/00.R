#euristic

n=3
x=seq(length=3)
f=function(n,x){
  a=0
  for(i in 1:n){
    a = a + i * x[i] *x[i];
  }
  return(a);
}

fm=10000000
euristic=function(n){
  for(i in 1:n){
    x[i]=runif(1, -10, 10)
  }
  if( f( n , x ) < fm ){
    fm = f( n , x )
  }
  return(fm)
}
c=seq(length=1000000)
d=seq(length=30)
for(j in 1:30){
  for(i in 1:1000000){
    c[i]=euristic(n)
  }
  d[j]=min(c)
}
print(mean(d))
print(min(d)) 
print(max(d))
for(i in 1:30)
  print(d[i])
for(i in 1:100000){
  c[i] = euristic(n)
}
min(c)
mean(c)
max(c)

#determinist

n=3
x=seq(length=3)
f=function(n,x){
  a=0
  for(i in 1:n){
    a = a + i * x[i] *x[i];
  }
  return(a);
}
for(i in 1:n){
  x[i]=0
}
minn=100000000
print(f(n,x))
determinist=function(n, nrparam, x,minn) {
  if(nrparam == 0){
    if(f(n, x) < minn){
      minn= f(n,x)
    }
  }
  else{
     i=-10;
    while( i<= 10 ){
      x[n-nrparam+1]=i;
      i=i+0.1
      determinist(n, nrparam-1, x, minn)
    }
  }
  }
nrparam=n
determinist(n, nrparam, x, minn)








