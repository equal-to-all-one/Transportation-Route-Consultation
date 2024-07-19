<template>
  <div>
    <h3>城市列表</h3>
    <el-row style="margin: 30px">
      <el-col :span="7">
        <el-select
          v-model="searched_city"
          filterable
          placeholder="请输入待搜索城市名"
        >
          <el-option
            v-for="item in cityLs"
            :label="item.name"
            :value="item"
            :key="item.id"
          >
          </el-option>
        </el-select>
      </el-col>
      <el-col :span="7">
        <el-button
          type="primary"
          v-if="GLOBAL.hasLogin"
          @click="addFormVisible = true"
          >添加城市</el-button
        >
        <el-dialog title="增加城市" :visible.sync="addFormVisible">
          <el-form
            ref="addForm"
            :model="addForm"
            :rules="rules"
            label-width="150px"
          >
            <el-form-item label="城市名" prop="name">
              <el-input
                v-model="addForm.name"
                placeholder="请输入(不超过10个字)"
                style="width: 400px"
              ></el-input>
            </el-form-item>
            <h4>到其它城市的距离</h4>
            <div class="city-list">
              <el-form-item
                v-for="(item, idx) in cityLs"
                :key="item"
                :label="item.name"
                :prop="`.rawDistanceLs[${idx}].km`"
                :rules="[{ required: true, message: '不能为空' }]"
              >
                <el-input-number
                  :min="1"
                  :controls="false"
                  v-model="addForm.rawDistanceLs[idx].km"
                ></el-input-number>
              </el-form-item>
            </div>
            <el-form-item style="margin-top: 30px">
              <el-button type="primary" @click="submitForm('addForm')"
                >增加</el-button
              >
              <el-button @click="resetForm('addForm')">重置</el-button>
            </el-form-item>
          </el-form>
        </el-dialog>
      </el-col>
    </el-row>
    <aCity
      :cities="cityLs"
      :searched="searched_city"
      @cityDeleted="deleteAcity"
      style="margin-bottom: 50px"
    ></aCity>
  </div>
</template>

<script>
import aCity from "../components/aCity";

export default {
  name: "myCity",
  components: { aCity },
  data() {
    const citynameRule = (rule, value, callback) => {
      if (this.addForm.name.length > 10) {
        callback(new Error("城市名不超过十个字符"));
      } else {
        var reg = new RegExp(
          "([\u3002\uff1b\uff0c\uff1a\u201c\u201d\uff08\uff09\u3001\uff1f\u300a\u300b\uff01\u3010\u3011\uffe5])+",
          "g"
        );
        if (reg.test(this.addForm.name)) {
          callback(new Error("请勿输入标点"));
        } else {
          callback();
        }
      }
    };
    return {
      cityLs: [],
      distanceLs: [],
      searched_city: "",
      addFormVisible: false,
      addForm: {
        name: "",
        rawDistanceLs: [],
      },
      rules: {
        name: [
          { validator: citynameRule, trigger: "blur" },
          { required: true, message: "不能为空", trigger: "blur" },
        ],
      },
    };
  },
  watch: {
    cityLs(newVal) {
      this.addForm.rawDistanceLs = [];
      newVal.map(() => {
        this.addForm.rawDistanceLs.push({ km: undefined });
      });
    },
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
          this.$axios({
            method: "post",
            url: "/transportation/cities/",
            headers: {
              "Content-Type": "application/json",
              Authorization: `Bearer ${localStorage.getItem("admin_access")}`,
            },
            data: {
              name: this.addForm.name,
            },
          }).then((res) => {
            console.log(res.data);
            const list = [];
            this.cityLs.map((city, index) => {
              list.push({
                cityA: res.data.id,
                cityB: city.id,
                distance: this.addForm.rawDistanceLs[index].km,
              });
            });
            this.cityLs.push(res.data);
            console.log(list, "list");
            this.$axios({
              method: "post",
              url: "/transportation/distances/",
              headers: {
                "Content-Type": "application/json",
                Authorization: `Bearer ${localStorage.getItem("admin_access")}`,
              },
              data: list,
            });
            this.addFormVisible = false;
          });
          // alert("o(*￣▽￣*)ブ");
        } else {
          // this.$message.error("/(ㄒoㄒ)/~~");
          return false;
        }
      });
      this.resetForm(formName);
    },
    getDetailDistance() {},
    addDistance() {},
    resetForm(formName) {
      this.$refs[formName].resetFields();
    },
    deleteAcity() {
      this.$axios({
        method: "get",
        url: "/transportation/cities/",
        headers: {
          "Content-Type": "application/json",
        },
      }).then((res) => {
        this.cityLs = res.data;
        // console.log(this.cityLs);
      });
      // console.log("删了(p≧w≦q)");
    },
  },
};
</script>

<style scoped>
ul li {
  list-style-type: none;
}
ul {
  padding-inline-start: 20px;
  padding-inline-end: 20px;
}
.city-list {
  max-height: 400px;
  overflow-y: auto;
}
</style>