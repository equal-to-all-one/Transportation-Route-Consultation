<template>
  <div style="width: 400px">
    <h3>管理员登录</h3>
    <el-form ref="form" :model="form" label-width="80px">
      <el-form-item label="用户名">
        <el-input v-model="form.username"></el-input>
      </el-form-item>
      <el-form-item label="密码">
        <el-input v-model="form.password" show-password></el-input>
      </el-form-item>
      <el-form-item>
        <el-button type="primary" @click="submit">登录</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script>
// import axios from 'axios';
export default {
  name: "myLogin",
  data: function () {
    return {
      form: {
        username: "",
        password: "",
      },
    };
  },
  mounted() {
    if (this.GLOBAL.hasLogin) {
      localStorage.removeItem("admin_access");
      localStorage.removeItem("admin_refresh");
      // alert("已退出登录");
      this.$message.info("已退出登录");
      this.GLOBAL.hasLogin = false;
    }
  },
  methods: {
    submit: function () {
      this.$axios({
        method: "post",
        url: "/transportation/token/",
        headers: {
          "Content-Type": "application/json",
        },
        data: this.form,
      })
        .then((res) => {
          // console.log(res.data);
          localStorage.setItem("admin_access", res.data.access);
          localStorage.setItem("admin_refresh", res.data.refresh);
          this.$message.info("登录成功");
          this.GLOBAL.hasLogin = true;
          this.$router.push({ name: "top" });
        })
        .catch((error) => {
          console.log(error);
          this.$message.warning("密码错误");
        });
    },
  },
};
</script>
