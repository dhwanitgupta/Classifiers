import numpy;
t = numpy.matrix([[-3.27,0.16,0.38,-0.21,1.35]]) 
t1 = numpy.matrix([[2.35,-0.244,-0.81,0.7,-1.2 ]]) 
t2 = numpy.matrix([[-0.61,0.1181,0.472,-0.46,-0.0770 ]]) 
f = open('fold1','r');

text = f.read();

for line in text.split("\n"):
	p = line.split(" ");
	if len(p) > 2:
		print p,' ', t,' ',t1,' ', t2
		q = numpy.matrix([1,float(p[0]),float(p[1]),float(p[2]),float(p[3])]);
		print q*t.T , ' ' , q*t1.T , ' ',q*t2.T , ' ' , p[4]
	

