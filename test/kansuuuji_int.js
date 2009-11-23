try{
	void(alert);
}catch(e){
	alert = function(x){WScript.Echo(x)};
}

function ktoi(kansuuji){
	var kanji_int = {
		"一":1,
		"二":2,
		"三":3,
		"四":4,
		"五":5,
		"六":6,
		"七":7,
		"八":8,
		"九":9,
		"零":0,
		"壱":1,
		"弐":2,
		"参":3
	}
	replaced = kansuuji.replace(/[一二三四五六七八九零壱弐参]/g,function(x){return kanji_int[x]});
	if(replaced == "")return 0;
	return parseInt(replaced,10);
}
function sub1(str){
	var s = ["千","百","十"];
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
	var s = ["兆","億","万"];
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


App.Alert(sub2("一二三四五六七八九"));
App.Alert(sub2("千百十一億"));
/*
二三四千

兆億万
千百十(拾)


零
壱
弐
参
*/
