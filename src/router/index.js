import Vue from 'vue'
import VueRouter from 'vue-router'
import myTop from '../views/myTop'
import myLogin from '../views/myLogin'
import myCity from '../views/myCity'
import myFlight from "../views/myFlight"

Vue.use(VueRouter);

export default new VueRouter({
    routes:[
        {
            path:'/',
            redirect:'/top'
        },
        {
            path:'/top',
            name:'top',
            component:myTop
        },
        {        
            path:'/login',
            name:'login',
            component:myLogin
        },
        {
            path:'/cities',
            name:'cities',
            component:myCity
        },
        {
            path:'/flights',
            name:'flights',
            component:myFlight
        }
    ]
})