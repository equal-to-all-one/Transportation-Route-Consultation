<template>
  <div>
    <el-menu class="el-menu-demo" mode="horizontal" style="width: 100%">
      <el-row :gutter="1">
        <el-col :span="4" style="margin-left:50px">
          <el-menu-item>
            <router-link to="/top"><h1 style="color:brown">交通咨询系统</h1></router-link>
          </el-menu-item>
        </el-col>
        <el-col :span="3" style="float: right;padding-right:50px">
          <el-menu-item v-if="GLOBAL.hasLogin">
            <router-link to="/login"><h2>管理员登出</h2></router-link>
          </el-menu-item>
          <el-menu-item v-else>
            <router-link to="/login"><h2>管理员登录</h2></router-link>
          </el-menu-item>
        </el-col>
        <el-col :span="2">
          <el-menu-item>
            <router-link to="/cities"><h2>城市</h2></router-link>
          </el-menu-item>
        </el-col>
        <el-col :span="2">
          <el-menu-item>
            <router-link to="/flights"><h2>航班</h2></router-link>
          </el-menu-item>
        </el-col>
      </el-row>
    </el-menu>
    <div style="padding:10px 200px">
    <router-view></router-view>
    </div>
  </div>
</template>

<script>
export default {
  name: "App",
  mounted() {
    let token = localStorage.getItem("admin_access");
    if (token) {
      console.log(token);
      this.GLOBAL.hasLogin = true;
    }
  },
};
</script>

<style scoped>
a {
  text-decoration: none;
}
.el-menu-demo {
  height: 100px;
}
</style>