// pages/mine/mine.js
const app = getApp()
const defaultAvatarUrl = 'https://mmbiz.qpic.cn/mmbiz/icTdbqWNOwNRna42FI242Lcia07jQodd2FJGIYQfG0LAJGFxM4FbnQP6yfMxBgJ0F3YRqJCJ1aPAK2dQagdusBZg/0'
Page({

  /**
   * 页面的初始数据
   */
  data: {
    avatarUrl: defaultAvatarUrl,
    nickName: '点击头像修改个人信息',
    deviceName: '无设备连接',
    isChanging: false,
    isAdding: false,
    atStep: 1,
    ip: '',
    port: '',
    ssid: '',
    pwd: '',
  },
  toChooseAvatar() {
    this.setData({
      isChanging: true,
    })
  },
  onChooseAvatar(e) {
    const { avatarUrl } = e.detail 
    this.setData({
      avatarUrl: avatarUrl,
    })
  },
  addDevice() {
    this.setData({
      isAdding: true
    })
  },
  closePopup() {
    this.setData({
      isAdding: false,
      atStep: 1,
      isChanging: false,
    })
  },
  nextStep() {
    this.setData({
      atStep: this.data.atStep+1
    })
  },
  getNickName(e) {
    this.setData({
      nickName: e.detail.value
    })
    console.log(this.data.nickName)
  },
  getIp(e) {
    this.setData({
      ip: e.detail.value
    })
    console.log(this.data.ip)
  },
  getPort(e) {
    this.setData({
      port: e.detail.value
    })
    console.log(this.data.port)
  },
  getSSID(e) {
    this.setData({
      ssid: e.detail.value
    })
    console.log(this.data.ssid)
  },
  getPWD(e) {
    this.setData({
      pwd: e.detail.value
    })
    console.log(this.data.pwd)
  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad(options) {

  },
  
  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady() {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow() {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide() {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload() {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh() {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom() {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage() {

  }
})