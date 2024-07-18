import Vue from 'vue/dist/vue.js';
import VueRouter from 'vue-router';
import axios from 'axios';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
import router from './router';
import App from './App';

Vue.use(VueRouter);
Vue.use(ElementUI);

Vue.config.productionTip = false

Vue.prototype.$axios = axios
axios.defaults.baseURL = 'http://localhost:8000/api/'

/* 拦截器 refresh token 但使用后无法返回数据？
axios.interceptors.response.use(
  function(response){
    return response.data;
  },
  async function(error){
    if(error.status===401){
      this.$axios({
        method:"post",
        url:"transportation/token/refresh/",
        headers: {
          "Content-Type": "application/json"
        },
        data:{
          "refresh":`Bearer ${localStorage.getItem('admin_refresh')}`
        }
      }).then((res)=>{
        localStorage.setItem('admin_access',res.data.access);
        localStorage.setItem('admin_refresh',res.data.refresh);
        error.response.config.headers['Authorization'] = `Bearer ${res.data.access}`;
        return axios(error.response.config);
      })
    }
  }
)*/

const loginData={
  hasLogin:false
};
Vue.prototype.GLOBAL=loginData;

new Vue({
  el:'#app',
  router,
  render: h => h(App),
})
