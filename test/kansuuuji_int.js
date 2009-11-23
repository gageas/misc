try{
	void(alert);
}catch(e){
	alert = function(x){WScript.Echo(x)};
}

function ktoi(kansuuji){
	var kanji_int = {
		"��":1,
		"��":2,
		"�O":3,
		"�l":4,
		"��":5,
		"�Z":6,
		"��":7,
		"��":8,
		"��":9,
		"��":0,
		"��":1,
		"��":2,
		"�Q":3
	}
	replaced = kansuuji.replace(/[���O�l�ܘZ���������Q]/g,function(x){return kanji_int[x]});
	if(replaced == "")return 0;
	return parseInt(replaced,10);
}
function sub1(str){
	var s = ["��","�S","�\"];
	var m = [1000,100,10];
	var i,index,lhs,rhs;
	for(i=0;i<s.length;i++){
		index = str.indexOf(s[i]);
		if(index != -1){
			lhs = ktoi(str.substr(0,index));
			rhs = sub1(str.substr(index+1));
			if(lhs == 0)lhs=1;
			return lhs*m[i]+rhs;
		}
	}
	return ktoi(str);
}
function sub2(str){
	var s = ["��","��","��"];
	var m = [1000000000000,100000000,10000];
	var i,index,lhs,rhs;
	for(i=0;i<s.length;i++){
		index = str.indexOf(s[i]);
		if(index != -1){
			lhs = sub1(str.substr(0,index));
			rhs = sub2(str.substr(index+1));
			if(lhs == 0)lhs=1;
			return lhs*m[i]+rhs;
		}
	}
	return sub1(str);
}


App.Alert(sub2("���O�l�ܘZ������"));
App.Alert(sub2("��S�\�ꉭ"));
/*
��O�l��

������
��S�\(�E)


��
��
��
�Q
*/
