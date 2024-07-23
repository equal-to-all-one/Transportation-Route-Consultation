<template>
  <div>
    <h3>路线查询</h3>
    <el-form
      ref="inquireForm"
      :model="inquireForm"
      :rules="rules"
      label-width="80px"
    >
      <el-row :gutter="10">
        <el-col :span="7">
          <el-form-item
            label="出发地"
            prop="departure_city"
            :rules="{ required: true, message: '不能为空', trigger: 'blur' }"
          >
            <el-select
              v-model="inquireForm.departure_city"
              filterable
              placeholder="请输入"
            >
              <el-option
                v-for="item in cityLs"
                :label="item.name"
                :key="item.id"
                :value="item.id"
              >
              </el-option>
            </el-select>
          </el-form-item>
        </el-col>
        <el-col :span="7">
          <el-form-item label="目的地" prop="destination_city">
            <el-select
              v-model="inquireForm.destination_city"
              filterable
              placeholder="请输入"
            >
              <el-option
                v-for="item in cityLs"
                :label="item.name"
                :key="item.id"
                :value="item.id"
              >
              </el-option>
            </el-select>
          </el-form-item>
        </el-col>
      </el-row>
      <el-row :gutter="1">
        <el-col :span="7">
          <el-form-item label="出行方式">
            <el-radio-group v-model="inquireForm.way">
              <el-radio label="1">飞机</el-radio>
              <el-radio label="2">高铁</el-radio>
            </el-radio-group>
          </el-form-item>
        </el-col>
        <el-col :span="7">
          <el-form-item label="是否中转">
            <el-switch v-model="inquireForm.transit"> </el-switch>
          </el-form-item>
        </el-col>
      </el-row>
      <el-row :gutter="1">
        <el-col :span="7">
          <el-form-item label="出发时间" prop="depart_time">
            <el-date-picker
              v-model="inquireForm.depart_time"
              type="datetime"
              placeholder="请选择"
              format="yyyy-MM-dd HH:mm"
              align="right"
            >
            </el-date-picker>
          </el-form-item>
        </el-col>
        <el-col :span="7">
          <el-form-item>
            <el-button type="primary" @click="submitForm('inquireForm')"
              >查询</el-button
            >
            <el-button @click="resetForm('inquireForm')">重置</el-button>
          </el-form-item>
        </el-col>
      </el-row>
    </el-form>
    <aRoute :routes="routeLs" style="margin-bottom: 50px"></aRoute>
  </div>
</template>

<script>
import aRoute from "../components/aRoute";

export default {
  name: "myTop",
  components: { aRoute },
  data() {
    const cityRule = (rule, value, callback) => {
      if (
        this.inquireForm.departure_city === this.inquireForm.destination_city
      ) {
        callback(new Error("起点终点不能相同"));
      } else {
        callback();
      }
    };
    const timeRule = (rule, value, callback) => {
      const inputTime = new Date(value);
      const currentTime = new Date();
      if (inputTime <= currentTime) {
        callback(new Error("出发时间不能早于现在"));
      } else {
        callback();
      }
    };
    return {
      cityLs: [],
      routeLs: [],
      inquireForm: {
        departure_city: "",
        destination_city: "",
        depart_time: "",
        way: "1",
        transit: true,
      },
      rules: {
        destination_city: [
          { required: true, message: "不能为空", trigger: "blur" },
          { validator: cityRule, trigger: "blur" },
        ],
        depart_time: [
          { required: true, message: "不能为空", trigger: "blur" },
          { validator: timeRule, trigger: "blur" },
        ],
      },
    };
  },
  mounted() {
    this.$axios({
      method: "get",
      url: "/transportation/cities/",
      headers: {
        "Content-Type": "application/json",
      },
    }).then((res) => {
      this.cityLs = res.data;
    });
  },
  methods: {
    submitForm(formName) {
      this.$refs[formName].validate((valid) => {
        if (valid) {
          let dt = new Date(this.inquireForm.depart_time);
          let year = dt.getFullYear();
          let month = (dt.getMonth() + 1).toString().padStart(2, "0");
          let date = dt.getDate().toString().padStart(2, "0");
          let hour = dt.getHours().toString().padStart(2, "0");
          let minute = dt.getMinutes().toString().padStart(2, "0");
          this.inquireForm.depart_time = `${year}-${month}-${date} ${hour}:${minute}`;
          this.$axios({
            method: "post",
            url: "/transportation/flights/inquire/",
            headers: {
              "Content-Type": "application/json"
            },
            data: this.inquireForm,
          }).then((res) => {
            this.routeLs = res.data;
            if (res.data.length === 0) {
              this.$alert("暂无路线");
            }
          });
        } else {
          return false;
        }
      });
    },
    resetForm(formName) {
      this.$refs[formName].resetFields();
      this.routeLs = [];
    },
  },
};
</script>