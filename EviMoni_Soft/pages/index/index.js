// index.js
// 获取应用实例
const app = getApp()

Page({
  data: {
    curPower: 99,
    curWater: 50,
    curTemp: 60,
    curLight: 100,
    PM: 0,
    youhai: 0,
    CO2: 0,
    quality: '差',
    aqi: 70,
    mess: '换个地方工作吧~',
    tmpMess: '水有点烫，等一会再喝吧！',
    lightMess: '光线充足，记得关灯！',
  },
  setPercentValue(value) {
    let cover = document.querySelector('.cover'),
        left = document.querySelector('.ring-left'),
        hide = document.querySelector('.ring-hide');
    let deg;//左环转动角度
    if (Number.isNaN(value) || value < 0) value = 0;
    if (value > 100) value = 100;
    if(value % 1 !== 0){  //若value为小数，保留一位小数
        value = value.toFixed(1);
    }
    cover.innerText = value + "%";
    if (value < 50) {  //值小于50，显示遮罩
        hide.style.display = "inherit";
        deg = (50 - value)/50 * 180;
    } else {  //值大于或等于50，不显示遮罩
        hide.style.display = "none";
        deg =  (50 - (value - 50))/50 * 180;
    }
    left.style.transform = "rotate("+ (-deg) +"deg)";
  },
  onLoad() {
    if (wx.getUserProfile) {
      this.setData({
        canIUseGetUserProfile: true
      })
    }
  },
 
})